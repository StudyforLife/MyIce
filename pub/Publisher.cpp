#include<iostream>
#include<IceStorm/IceStorm.h>
#include<Ice/Ice.h>
#include<sstream>
#include<vector>
#include<time.h>
#include<cstdio>
#include<sys/types.h>
#include<sys/stat.h>
#include"Station.h"

using namespace std;
using namespace StationDemo;


myTopicList create_topic()
{
	myTopicList m;
	int size=100;
	stringstream ss;
	m.reserve(size);
	for(int i=1;i<=size;i++)
	{
		ss<<i;
		string s="主题"+ss.str();
		m.push_back(s);
		ss.clear();
		ss.str("");
	}
	return m;
}

vector<int> create_Data(myTopicList m)
{
	vector<int> v;
	int sum=0;
	for(int i=1;i<=m.size();i++)
	{
		sum+=i;
		v.push_back(sum);
	}
	return v;
}



int main(int argc,char* argv[])
{	

	myTopicList m=create_topic();
	Ice::CommunicatorPtr ic;

	//读取配置文件
	Ice::InitializationData initData;
	initData.properties=Ice::createProperties();
	initData.properties->load("Config/config.client");
	ic=Ice::initialize(initData);

	Ice::ObjectPrx obj=ic->propertyToProxy("client.proxy");
	/*
	//直接初始化
	ic=Ice::initialize();
	
	Ice::ObjectPrx obj=ic->stringToProxy("StationTest/TopicManager:default -h localhost -p 9999");*/

	IceStorm::TopicManagerPrx topicManager=IceStorm::TopicManagerPrx::checkedCast(obj);

	//存储主题代理
	IceStorm::TopicPrx TopicList[m.size()];
	for(int i=0;i<m.size();i++)
	{
		try{
			TopicList[i]=topicManager->retrieve(m[i]);
		}catch(const IceStorm::NoSuchTopic&){
			try{
				TopicList[i]=topicManager->create(m[i]);
			}catch(const IceStorm::TopicExists&){
				//error
			}
		}
	}

	StationMessagePrx station[m.size()];
	for(int j=0;j<m.size();j++)
	{
		Ice::ObjectPrx pub=TopicList[j]->getPublisher();
		station[j]=StationMessagePrx::uncheckedCast(pub);	
	}


	//操作调用
	time_t t;
	char str[255]={};
	vector<int> v1=create_Data(m);
	Message msg;
	stringstream ss;
	msg.flag=0;
	while(true)
	{
		
		msg.flag+=1;
		for(int i=0;i<m.size();i++)
		{	
			time(&t);
			tm* p=localtime(&t);
			sprintf(str,"%4d/%d/%d-%d:%d:%d",p->tm_year+1900,\
			p->tm_mon+1,p->tm_mday,p->tm_hour,p->tm_min,p->tm_sec);
			string message="江陵路-近江-远江-边疆-新疆-下沙-上沙-中沙-自定义消息";
			msg.tpName=m[i];
			ss<<v1[i];
			msg.data=ss.str()+": "+message;
			ss.clear();
			ss.str("");
			msg.time=str;
			int size1=msg.data.length()+msg.tpName.length()+msg.time.length();
			cout<<"数据大小："<<size1<<endl;
			
			station[i]->getMsg(msg);
			usleep(30000);
			//sleep(5);
		}
			cout<<"flag: "<<msg.flag<<endl;
			while(msg.flag>=1000)
			{
				msg.flag=0;
			}
	
	}
	if(ic)
		ic->destroy();
}
