# Sms  Controlled  Sensor/Relay ![emoji_u1f4e8](https://user-images.githubusercontent.com/43998896/148659460-7c845db6-0e66-4736-8577-2e5ae3b836d3.png) 



<img src="Schema.png" width=700/>


## MOTIVATION

A friend of mine has got a countryside house.
He wants to control a boiler, especially in winter.
But the house is distant and the Internet is not provided.
For him we designed a device which replies on the SMS (with a token) 
sending back the requested data. In my friends' case - temperature measured.


## INGREDIENTS

- [Arduino Nano](https://arduino.ua/prod2177-arduino-nano-v3-0-avr-atmega328-p-20au)
- [SIM800L](https://arduino.ua/prod1665-gsm-modyl-na-sim800l)
- [SHT20 I2C](https://arduino.ua/prod4499-modyl-datchika-temperatyri-i-vlajnosti-sht20-i2c)
- Power supply and miscellaneous


## [SIM800L DOCUMENTATION](SIM800%20Series_AT%20Command%20Manual_V1.12.pdf)


## COOKBOOK 

First, copy and import SMS800L.h file:
```c++
#include "SMS800L.h"
```
Second, redefine a method inCycle, adding your cases:
```c++
class MySMS : public SMSManager {           // <----inherit 
   public:
	using SMSManager::SMSManager;

	void inCycle() {                    // <----redefine
	SMSManager::inCycle();

	/// Case1-> If SMS contains the first token   
	if (strstr(this->getRowData(), TOKEN1))  { 
	        //switch a relay, ask sensors, etc...  
                //then create a resonse
		this->sms(RESPONSE1, this->getNumber()); 
	}

	/// Case2-> If SMS contains the next token   
	if (strstr(this->getRowData(), TOKEN2))  {...}	
                     
                     
        /// Case3...
};

```

Third, instantiate the class:

```c++
	///settings for SIM800L
	const char * initString [] ={"AT+CMGF=1", "AT+CNMI=1,2,0,0,1", \
	                            "AT+CMGDA=\"DEL ALL\"", "AT+GSMBUSY=1"}
				    
	MySMS smska (&altSerial, &Serial, initString, 5000);
	///arguments: 
	/// 1) pointer at AltSoftSerian or SoftwareSerial to comuunicate with a SIM800L board
	/// 2) pointer at Serial to comunicate with PC (peripheral)
	/// 3) char [][] - array of settings
	/// 4) timeout while waiting SIM800L response, milliseconds
	
```

Finally, add a call to Arduino loop:

```c++
void loop() {   
	smska->inCycle();
	///rest of the work
	///....
} 


```


## ENJOY
We hope you will be interested in repeating this project

## TODO
To program the watchdog
