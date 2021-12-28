# GiveMeTempSMS


<img src="Schema.png" width=500/>


## MOTIVATION

A friend of mine has got a countryside house.
He wants to control a boiler, especially in winter.
But the house is distant and Internet is not provided.
So we designed a device which replies on a code SMS with 
requested information. In my friends' case, with temperature measured.


## INGREDIENTS

- [Arduino Nano](https://arduino.ua/prod2177-arduino-nano-v3-0-avr-atmega328-p-20au)
- [SIM800L](https://arduino.ua/prod1665-gsm-modyl-na-sim800l)
- [SHT20 I2C](https://arduino.ua/prod4499-modyl-datchika-temperatyri-i-vlajnosti-sht20-i2c)
- Power supply and miscellaneous


## YOUR ADDITIONS

You can create any action youself following the snippet bellow
```c++
       //If SMS contains a specific code string - CODESMS         
       if (strstr(inputString, CODESMS))  {  
          if (parseNum(inputString)) {
            // here do some job and get RESULT:
            // - ask sensors to measure
            // - turn on/off relay, etc.  
            // and report to sender
            sms(String(RESULT, outnum); 
          }
       }
```

## Enjoy
We hope you will be interested in repeating this project
