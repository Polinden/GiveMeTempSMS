#include "gtest/gtest.h"
#include "lib/SIM800L.h"

TEST(GetRowString, SMS800L) {
	SMSManager simka;
	char * str1="FFGHJKL";
        strcpy(simka.inputString, str1);
        char* expected = "FFGHJKL";
        ASSERT_STREQ(expected,  simka.getRowData());
}

TEST(GetBadNumber, SMS800L) {
	SMSManager simka;
	char * str1="+CMT:ewfgrehtrjy+3q0503118207\n\rjhka\n\rlnvdbmd;g";
        strcpy(simka.inputString, str1);
        char* expected = "";
        simka.getNumber();
        ASSERT_STREQ(expected,  simka.getNumber());
}

TEST(GetGoodNumber, SMS800L) {
	SMSManager simka;
	char * str1="+CMT:ewfgrehtrjy+380503118207\n\rjhka\n\rlnvdbmd;g";
        strcpy(simka.inputString, str1);
        char* expected = "+380503118207";
        simka.getNumber();
        ASSERT_STREQ(expected,  simka.getNumber());
}


TEST(GetGoodVolt, SMS800L) {
	SMSManager simka;
	char * str1="\r\njj+CBC:123,123,3445\ndg dfgqe\n";
        strcpy(simka.inputString, str1);
        char* expected = "3445";
        int res=simka.parseVolt();
        ASSERT_STREQ(expected,  simka.voltageString);
}


TEST(GetBadVolt, SMS800L) {
	SMSManager simka;
	char * str1="\r\njj+CWC:123,123,3445\ndg dfgqe\n";
        strcpy(simka.inputString, str1);
        char* expected = "";
        int res=simka.parseVolt();
        ASSERT_STREQ(expected,  simka.voltageString);
}
