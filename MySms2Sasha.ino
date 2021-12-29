#include <AltSoftSerial.h>
#include "DFRobot_SHT20.h"
#include <Wire.h>
#include <stdio.h>
#include <string.h>

#define CODESMS "MISH"
#define CODEHEALTH "VOLT"
#define PHRASE "Hello, Dude! Temp(C)="
#define HPHRASE "Hello, Dude! Power(mV)="
#define VOLTSIGN "+CBC:"
#define NUMSIGN "+380"
#define BUFFSIZE 500
#define TIMEOUT 10000

//#define DEDUG
#ifdef DEDUG
#define LOG_ON(x,y) debugLog(x,y)
#else
#define LOG_ON(x,y)
#endif




 AltSoftSerial altSerial;
 DFRobot_SHT20 sht20;
 char outnum [15];
 char outvolt [15];
 char incomingByte; 
 char inputString [BUFFSIZE+1];
 float t;


 
 void sms(String text, const char * phone)  
 {
   char * phones="AT+CMGS=\"+380663539598\"\0";
   memcpy(phones+9, phone, 13);
   altSerial.println(phones);
   waitOK();
   readData();
   altSerial.print(text);
   delay(200);
   altSerial.print((char)26);
   waitOK();
   readData();
   LOG_ON("SMS's sent to: ", phones); 
 } 


int parseNum(const char * s1){
  char * s;
  char number [14];
  s = strstr(s1, "+CMT:");
  if (s) s = strstr(s, NUMSIGN);
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
  s = strstr(s1, VOLTSIGN);
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




void loop() {   
   if(altSerial.available()){   
       waitOK();                                     
       readData();  
        
       //
       //Actions list:
       //
    
       // - Case-1 (Measure temp)         
       if (strstr(inputString, CODESMS))  { 
          LOG_ON("input: ", inputString);  
          int res=parseNum(inputString);
          if (res) {
            LOG_ON("number: ", outnum);  
            t = sht20.readTemperature();      
            sms(String(PHRASE+String(round(t))), outnum); 
          }
       }
       
       // - Case-2 (HealthChech)
       if (strstr(inputString, CODEHEALTH))  {   
          LOG_ON("input: ", inputString);  
          int res=parseNum(inputString);
          if (res) {
             LOG_ON("number: ", outnum);      
             altSerial.println("AT+CBC");  //Check Voltage  
             waitOK();
             readData();
             int res=parseVolt(inputString);
             if (res) {
             LOG_ON("voltage: ", outvolt);
             sms(HPHRASE+String(outvolt), outnum);
             }
          } 
       }
       
       // - Default case (Clear SMSs and buffer)
       if (!strstr(inputString, "OK")){    
        altSerial.println("AT+CMGDA=\"DEL ALL\"");      
        waitOK(); 
        readData();
       }     
    }
}



void debugLog(const char * s, const char * c){
           Serial.write(s, strlen(s));
           Serial.write(c, strlen(c));
           Serial.print('\n');
}
