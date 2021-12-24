#include <AltSoftSerial.h>
#include "DFRobot_SHT20.h"
#include <Wire.h>
#include <stdio.h>
#include <string.h>
#define CODESMS "MISH"

 char incomingByte; 
 String inputString;
 AltSoftSerial altSerial;
 DFRobot_SHT20 sht20;
 const char * pat1="+380";
 char outnum [15];
 String number;
 float t;

 

 void sms(String text, String phone)  // Процедура Отправка SMS
 {
   Serial.println("SMS send started");
   altSerial.println("AT+CMGS=\"" + phone + "\"");
   delay(500);
   altSerial.print(text);
   delay(500);
   altSerial.print((char)26);
   delay(500);
   Serial.println("SMS send complete");
   delay(2000);
 } 

void setup() {
 Serial.begin(9600);
 while (!Serial) ; 
 Serial.println("SHT31 test");
 sht20.initSHT20(); 
 delay(100);
 sht20.checkSHT20();
 delay(200); 
 Serial.println("AltSoftSerial Test Begin");
 altSerial.begin(9600);
 while(!altSerial.available()){               // Init SIM800L
    altSerial.println("AT");                   
    delay(1000);                              
    Serial.println("Connecting…");             
  }
  Serial.println("Connected!");               
  delay(20000); 
  altSerial.println("AT+CMGF=1");             // Send AT+CMGF=1
  delay(1000);                                 
  altSerial.println("AT+CNMI=1,2,0,0,0");     // Modes AT+CNMI=1,2,0,0,0
  delay(1000);                                 
  altSerial.println("AT+CMGL=\"REC UNREAD\""); 
  delay(1000);  
}


int parseNum(char * s1){
  char * s;
  char number [14];
  s = strstr(s1, pat1);
  if(s) {
    strncpy(number, s, 14);
    snprintf (outnum, 14, "%s", s);
    return 1;
  }
  else return 0;
}


void loop() {   
   if(altSerial.available()){   
      t = sht20.readTemperature();             
      delay(200);                             
      while(altSerial.available()){            
         incomingByte = altSerial.read();          
         inputString += incomingByte;              
      }   
       delay(50);                                
       if (inputString.indexOf(CODESMS) > -1)  {   
          Serial.println(inputString);
          int res=parseNum(inputString.c_str());

          if (res) {
             number=String(outnum);
             Serial.println(number);
             sms(String("Hello, Mother Fucker! Temp="+String(round(t))), String(number)); 
          }
          delay(50);
       }
       if (inputString.indexOf("OK") == -1){    
        altSerial.println("AT+CMGDA=\"DEL ALL\"");      
        delay(1000);
       }     
       inputString = "";
    }
}
