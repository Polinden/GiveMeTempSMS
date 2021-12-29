
#include <stdio.h>
#include <string.h>

class SMSManager {
#define BUFFSIZE 100
#define VOLTSIGN "+CBC:"
#define NUMSIGN "+380"
#define LOG_ON(x,y)

	public:
	SMSManager () 
	{
	}

	const char * getNumber(){
		if (!parsed) parseNum();
		return numberString;
	}


	const char * getRowData(){
		return inputString;
	}


	void readData(bool waited){
		parsed=false;
	}


	char incomingByte; 
	char inputString [BUFFSIZE+1];
	char numberString [15];
	char voltageString [11];
	bool parsed=false;

	int parseVolt(){
		char * s, * e;
		char number [14];
                voltageString[0]='\0';
		s = strstr(inputString, VOLTSIGN);
		if (s) s = strstr(s+1, ",");
		if (s) s = strstr(s+1, ",");
		if (s) e = strstr(s+1, "\n");
		if (s && e && (int) (e-s)>10) e = s + 10;
		if(s && e) {
			strncpy(number, s+1, (int) (e-s));
			snprintf(voltageString,  (int) (e-s), "%s", number);
			LOG_ON("Voltage: ", voltageString); 
			return 1;
		}
		LOG_ON("Error: ", "nuknown voltage"); 
		return 0;
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

};


