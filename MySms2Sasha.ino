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
#define TIMEOUT 10000


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
				char tempstr[10];  
				dtostrf(temp, 6, 1, tempstr); 
				snprintf(answer, PHRASELEN-1, PHRASE, tempstr);    
				this->sms(answer, this->getNumber()); 
			}

			//Case2 (HealthChech)
			if (strstr(this->getRowData(), CODEHEALTH)) this->checkHealth();

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
	smska->inCycle(); 
} 
