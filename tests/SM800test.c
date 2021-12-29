    #include "CuTest.h"
    #include "SMS800.h"



    void TestGet(CuTest *tc) {
	SMSManager simka;
	char * str1="FFGHJKL"; 
        strcpy(simka.inputString, str1);
        char* expected = "FFGHJKL";
        CuAssertStrEquals(tc, expected,  simka.getRowData());
    }

     void TestParseNoNum(CuTest *tc) {
	SMSManager simka;
	char * str1="+CMT:ewfgrehtrjy+3q0503118207\n\rjhka\n\rlnvdbmd;g"; 
        strcpy(simka.inputString, str1);
        char* expected = "";
        simka.getNumber();
        CuAssertStrEquals(tc, expected,  simka.getNumber());
    }

     void TestParseNum(CuTest *tc) {
	SMSManager simka;
	char * str1="+CMT:ewfgrehtrjy+380503118207\n\rjhka\n\rlnvdbmd;g"; 
        strcpy(simka.inputString, str1);
        char* expected = "+380503118207";
        simka.getNumber();
        CuAssertStrEquals(tc, expected,  simka.getNumber());
    } 


     void TestParseVolt(CuTest *tc) {
	SMSManager simka;
	char * str1="\r\njj+CBC:123,123,3445\ndg dfgqe\n"; 
        strcpy(simka.inputString, str1);
        char* expected = "3445";
        int res=simka.parseVolt();
        CuAssertStrEquals(tc, expected,  simka.voltageString);
    }  

     void TestNoParseVolt(CuTest *tc) {
	SMSManager simka;
	char * str1="\r\njj+CWC:123,123,3445\ndg dfgqe\n"; 
        strcpy(simka.inputString, str1);
        char* expected = "";
        int res=simka.parseVolt();
        CuAssertStrEquals(tc, expected,  simka.voltageString);
    }  

    CuSuite* StrUtilGetSuite() {
        CuSuite* suite = CuSuiteNew();
        SUITE_ADD_TEST(suite, TestGet);
        SUITE_ADD_TEST(suite, TestParseNum);
        SUITE_ADD_TEST(suite, TestParseNoNum);
        SUITE_ADD_TEST(suite, TestParseVolt);
        SUITE_ADD_TEST(suite, TestNoParseVolt);
        return suite;
    }

