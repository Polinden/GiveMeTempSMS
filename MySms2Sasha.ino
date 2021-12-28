#include <AltSoftSerial.h>
#include "DFRobot_SHT20.h"
#include <Wire.h>
#include <stdio.h>
#include <string.h>
#define CODESMS "MISH"
#define CODEHEALTH "VOLT"
#define PHRASE "Hello, Dude! Temp(C)="
#define HPHRASE "Hello, Dude! Power(mV)="
#define BUFFSIZE 500
#define TIMEOUT 10000


 AltSoftSerial altSerial;
 DFRobot_SHT20 sht20;
 const char * pat_num="+380";
 const char * pat_volt="+CBC:";
 char outnum [15];
 char outvolt [15];
 char incomingByte; 
 char inputString [BUFFSIZE+1];
 float t;

 

 void sms(String text, String phone)  
 {
   altSerial.println("AT+CMGS=\"" + phone + "\"");
   waitOK();
   readData();
   altSerial.print(text);
   delay(200);
   altSerial.print((char)26);
   waitOK();
   readData();
   #ifdef DEBUG
    Serial.println("SMS sended");
   #endif
   
 } 

void setup() {
 Serial.begin(9600);
 while (!Serial) ; 
 Serial.println("SHT31 test");
 sht20.initSHT20(); 
 delay(300);
 sht20.checkSHT20();
 delay(300); 
 altSerial.begin(9600);
 while(!altSerial.available()){               
    altSerial.println("AT");                   
    delay(1000);                                      
  }
  Serial.println("Altsoftserial Connected!"); 
  for (byte i=0; i<20; i++){
         delay(1000); 
         Serial.print('.');       
  }
  Serial.print('\n');
  altSerial.println("AT+CMGF=1");    //Text mode         
  waitOK();         
  readData();                        
  altSerial.println("AT+CNMI=1,2,0,0,1");   //Format read  
  waitOK();    
  readData();                             
  altSerial.println("AT+CMGDA=\"DEL ALL\"");  //Clear untread
  waitOK();  
  readData(); 
  altSerial.println("AT+GSMBUSY=1");  //No call
  waitOK(); 
  readData();
  if (strstr(inputString, "OK")) Serial.println("Setup finished! Working cycle...");
  else Serial.println("Setup failed! :(");
  
}


int parseNum(const char * s1){
  char * s;
  char number [14];
  s = strstr(s1, "+CMT:");
  if (s) s = strstr(s, pat_num);
  if(s) {
    strncpy(number, s, 14);
    snprintf (outnum, 14, "%s", number);
    return 1;
  }
  else return 0;
}


int parseVolt(const char * s1){
  char * s, * e;
  char number [14];
  s = strstr(s1, pat_volt);
  if (s) s = strstr(s+1, ",");
  if (s) s = strstr(s+1, ",");
  if (s) e = strstr(s+1, "\n");
  if (s && e && (byte) (e-s)>10) e = s + 10;
  if(s && e) {
    strncpy(number, s+1, (byte) (e-s+1));
    snprintf (outvolt,  (byte) (e-s+1), "%s", number);
    return 1;
  }
  else return 0;
}


void readData(){
      int is=0;
      while(altSerial.available() && is<BUFFSIZE){            
         incomingByte = altSerial.read();          
         inputString[is++] = incomingByte;              
      }  
      inputString[is]='\0';
}


void waitOK(){
  unsigned long _timeout = millis() + TIMEOUT;
  while (!altSerial.available() && millis() < _timeout)  {};  
  delay(300);                                         
}


void loop() {   
   if(altSerial.available()){   
       t = sht20.readTemperature();  
       waitOK();                                     
       readData();   
       //Case1 (command to measure)         
       if (strstr(inputString, CODESMS))  { 
          #ifdef DEBUG
           Serial.print("input: ");  
           Serial.write(inputString, strlen(inputString));
           Serial.print('\n');
          #endif
          
          int res=parseNum(inputString);
          if (res) {
            #ifdef DEBUG
             Serial.print("number: ");
             Serial.write(outnum, strlen(outnum));
             Serial.print('\n');
            #endif       
             sms(String(PHRASE+String(round(t))), String(outnum)); 
          }
       }
       //Case2 (HealthChech)
       if (strstr(inputString, CODEHEALTH))  {   
         #ifdef DEBUG
          Serial.print("input: ");  
          Serial.write(inputString, strlen(inputString));
          Serial.print('\n');
         #endif
           
          int res=parseNum(inputString);
          if (res) {
            #ifdef DEBUG
             Serial.print("number: ");
             Serial.write(outnum, strlen(outnum));
             Serial.print('\n');
            #endif        
             altSerial.println("AT+CBC");  //Check Voltage  
             waitOK();
             readData();
             int res=parseVolt(inputString);
             if (res) {
             #ifdef DEBUG
              Serial.print("voltage: ");
              Serial.write(outvolt, strlen(outvolt));
              Serial.print('\n');
             #endif  
              sms(HPHRASE+String(outvolt), String(outnum));
             }
          } 
       }
       //Clear SMSs and buffer
       if (!strstr(inputString, "OK")){    
        altSerial.println("AT+CMGDA=\"DEL ALL\"");      
        waitOK(); 
        readData();
       }     
    }
}
