#include<iostream>
#include<fstream>
#include<iomanip>
#include"myFile.h"
myFile::myFile(char* s)
{
	filename=s;
	file.open(filename,ios::out|ios::app);
}

void myFile::writeLog(Message msg)
{
	file<<msg.tpName<<"     "<<msg.data<<"   "<<msg.time<<endl;
}

void myFile::writeError(string str1,string str2,string str3)
{
	file<<"数据来源："<<str1<<"   "<<"数据目的地： "<<"  "<<\
	str2<<"丢失时间："<<str3<<endl;
}

void myFile::initLog()
{
	file.seekg(0,ios::end);
	streampos pos=file.tellg();
	if(int(pos)==0){
		string s1="--主题名--";
		string s2="--数据--";
		string s3="--发送时间--";
		file<<setw(15)<<s1<<setw(15)<<s2<<setw(20)<<s3<<endl;
	}else{return;}
}

void myFile::closeLog()
{
	file.close();
}


