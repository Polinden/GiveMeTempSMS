#include <AltSoftSerial.h>
#include "DFRobot_SHT20.h"
#include <Wire.h>
#include <stdio.h>
#include <string.h>
#include "SM800L.h"

#define CODESMS "MISH"
#define CODEHEALTH "VOLT"
#define PHRASE "Hello, Dude! Temp =%sC"
#define PHRASELEN 35


AltSoftSerial altSerial;
DFRobot_SHT20 sht20;
double temp;


class MySMS : public SMSManager {
    public:
    using SMSManager::SMSManager;
    
    void inCycle(){
       SMSManager::inCycle();
       
      //Case1 (command to measure)   
       if (strstr(this->getRowData(), CODESMS))  { 
        temp = sht20.readTemperature();
        char answer[PHRASELEN];
        char outstr[10];  
        dtostrf(temp, 6, 1, outstr); 
        snprintf(answer, PHRASELEN-1, PHRASE, outstr);    
        this->sms(answer, this->getNumber()); 
       }

<<<<<<< HEAD
      //Case2 (HealthChech)
        if (strstr(this->getRowData(), CODEHEALTH)) this->checkHealth();
=======
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
>>>>>>> b6af7469d05d5b1735cf4f9ad76ad964dd8412d9

      //Default case (Clear SMSs and buffer)
        if (this->getRowData()[0] != '\0' && !strstr(this->getRowData(), "OK")) this->clearSMS();
    }
};


MySMS * smska=0;
void setup() {
	Serial.begin(9600);
	while (!Serial) ; 
	sht20.initSHT20(); 
	delay(300);
	sht20.checkSHT20();
	delay(300); 
	altSerial.begin(9600);
	Serial.println("Wait for net connection!"); 
	for (byte i=0; i<20; i++){
    Serial.print('.');   
		delay(1000);     
	};
  Serial.println('.'); 
	const char * initString [] ={"AT+CMGF=1", "AT+CNMI=1,2,0,0,1", "AT+CMGDA=\"DEL ALL\"", "AT+GSMBUSY=1"};
	smska=new MySMS(&altSerial, &Serial, initString, 4, TIMEOUT) ;
	Serial.println("Setup finished! Working cycle...");  
} 




void loop() {   
<<<<<<< HEAD
		smska->inCycle();  
=======
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
>>>>>>> b6af7469d05d5b1735cf4f9ad76ad964dd8412d9
}
