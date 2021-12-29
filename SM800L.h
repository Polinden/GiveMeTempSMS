
class SMSManager {
//#define DEDUG
#ifdef DEDUG
#define LOG_ON(x,y) debugLog(x,y)
#else
#define LOG_ON(x,y)
#endif 
#define BUFFSIZE 200
#define VOLTSIGN "+CBC:"
#define NUMSIGN "+380"
#define TIMEOUT 10000
#define HPHRASE "HealthCheck! Power = %s%s"

	Stream * sm_w, * sm_d;
	long timeout;

	public:
	SMSManager (Stream *stm_w, Stream *stm_d, const char * conf [], int num_str, long timeout) 
	: sm_w { stm_w },     
		sm_d { stm_d },
		timeout { timeout } ,
		inputString {"\0"},
    parsed {false}
    
	{
		while(!sm_w->available()){               
			printLn("AT", sm_w);                   
			delay(500);                                      
		}
		for (int i=0; i<num_str; i++){
			printLn(conf[i], sm_w);                     
			readData();
			}
      if (!strstr(inputString, "OK")) {
        printLn("Setup failed! :(", sm_d);  
        printLn(inputString, sm_d);  
        return; 
      }
			printLn("Setup done :)", sm_d);     
	}

	void sms(String text, const char * phone)  
	{
		char * phones="AT+CMGS=\"+380663539598\"\0";
		memcpy(phones+9, phone, 13);
		printLn(phones, sm_w);
		readData();
		sm_w->print(text);
		delay(200);
		sm_w->print((char)26);
		readData();
		if (!strstr(inputString, "OK")) LOG_ON("SMS sended to: ", phone); 
		else LOG_ON("Error SMS: ", inputString); 
	} 

	const char * getNumber(){
		if (!parsed) parseNum();
		return numberString;
	}


	const char * getRowData(){
		return inputString;
	}


	void readData(){
		int is=0; 
		waitOK();
	  while(sm_w->available() && is<BUFFSIZE){            
			     incomingByte = sm_w->read();          
			     inputString[is++] = incomingByte;              
		}  
		inputString[is]='\0';
		LOG_ON("input: ", inputString);  
		parsed=false;
	}


	void checkHealth(){         //Check Voltage  
    char num [15];
    char outstr[40];  
    strcpy(num, getNumber());
		printLn("AT+CBC", sm_w);  
		readData();
    if (parseVolt()) {
      snprintf(outstr, 39, HPHRASE, voltageString, "mV");  
		  sms(outstr, num);
    }
	} 


	void clearSMS(){
		printLn("AT+CMGDA=\"DEL ALL\"", sm_w);      
		readData();
	} 

  void inCycle(){
    inputString[0]='\0';
    if(!sm_w->available()) return;      
    readData();
  } 

	private: 
	char incomingByte; 
	char inputString [BUFFSIZE+1];
	char numberString [15];
	char voltageString [11];
	bool parsed;

	void printLn(const char * text, Stream * st){
		st->print(text);
		st->print('\n');
	}

	int parseVolt(){
		char * s, * e;
		char number [14];
    voltageString[0]='\0';
		s = strstr(inputString, VOLTSIGN);
		if (s) s = strstr(s+1, ",");
		if (s) s = strstr(s+1, ",");
		if (s) e = strstr(s+1, "\n");
		if (s && e && (byte) (e-s)>10) e = s + 10;
		if(s && e) {
			strncpy(number, s+1, (byte) (e-s-1));
			snprintf(voltageString,  (byte) (e-s-1), "%s", number);
			LOG_ON("Voltage: ", voltageString); 
			return 1;
		}
		LOG_ON("Error: ", "nuknown voltage"); 
		return 0;
	}

	int waitOK(){
		unsigned long _timeout = millis() + TIMEOUT;
		while (!sm_w->available() && millis() < _timeout)  {};  
    if (!sm_w->available()) return 0;
		delay(400);                     
    return 1;                    
	}

	int parseNum(){
		char * s;
		char number [14];
    numberString[0]='\0';
		s = strstr(inputString, "+CMT:");
		if (s) s = strstr(s, NUMSIGN);
		if(s) {
			strncpy(number, s, 14);
			snprintf (numberString, 14, "%s", number);
			parsed=true;
			LOG_ON("number: ", numberString); 
			return 1;
		}
		LOG_ON("Error: ", "nuknown number"); 
		return 0;
	}


	void debugLog(const char * s, const char * c){
		Serial.write(s, strlen(s));
		Serial.write(c, strlen(c));
		Serial.print('\n');
	}
};
