#ifndef __MYFILE_H_
#define __MYFILE_H_
#include"Station.h"
#include<fstream>
using namespace std;
using namespace StationDemo;
class myFile{

public:
	myFile(char* s);
	void writeLog(Message msg);
	void writeError(string str1,string str2,string str3);
	void initLog();
	void closeLog();
private:
	const char* filename;
	fstream file;
};


#endif
