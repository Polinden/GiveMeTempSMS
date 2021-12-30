# GiveMeTempSMS


<img src="Schema.png" width=650/>


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


## YOUR ACTIONS

You can create any action youself following the snippet bellow
```c++
class MySMS : public SMSManager {           // <----inherit 
   public:
	using SMSManager::SMSManager;

	void inCycle(){                     // <----override
	SMSManager::inCycle();

	//Case1-> If SMS contains the first token   
	if (strstr(this->getRowData(), TOKEN1))  { 
	        //switch a relay, ask sensors, etc...  
                //then create a resonse
		this->sms(RESPONSE1, this->getNumber()); 
	}

	//Case2-> If SMS contains the next token   
	if (strstr(this->getRowData(), TOKEN2))  {...}	
                     
                     
        //Case3...
};

```

## Enjoy
We hope you will be interested in repeating this project

## TODO
To program the watchdog
