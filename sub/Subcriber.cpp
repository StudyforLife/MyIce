#include<iostream>
#include<Ice/Ice.h>
#include<IceStorm/IceStorm.h>
#include<iterator>
#include"Station.h"
#include"Menu.h"
#include"SubMenu.h"
#include"myFile.h"
#include<iomanip>
#include<map>
#include<sstream>
#include<time.h>
#include<signal.h>
#include<sys/time.h>
using namespace std;
using namespace StationDemo;

//存储定时检测标志
typedef struct signal_tmp{
	int last;
	int next;
}Temp;
map<string,Temp> singnal_map;
int cc=0;


//存储flag
map<string,int> flag_map;
//设置定时器
static struct itimerval oldtv;
void set_timer(){
	struct itimerval itv;
	itv.it_interval.tv_sec=5;
	itv.it_interval.tv_usec=0;
	itv.it_value.tv_sec=5;
	itv.it_value.tv_usec=0;
	setitimer(ITIMER_REAL,&itv,&oldtv);
}

void signal_func(int m)
{
	map<string,int>::iterator iter;
	map<string,Temp>::iterator it;
	for(iter=flag_map.begin();iter!=flag_map.end();++iter)
	{
		it=singnal_map.find(iter->first);
		if(it==singnal_map.end())
		{
			Temp tmp={0,0};
			singnal_map.insert(pair<string,Temp>(iter->first,tmp));
		}
		//结构体Temp 记录flag;
		if(it->second.last==0){
	
			it->second.last=iter->second;
		}else if(it->second.next==0){
			it->second.next=iter->second;
		}else{
			it->second.last=it->second.next;
			it->second.next=iter->second;
		}
	}

	if(it->second.last!=0 && it->second.next!=0)
	{
		if(it->second.last==it->second.next)
		{
			char log_loss[100]={"log_loss"};
			myFile file_loss(log_loss);
			string str2="连接正常";
			string str3="未收到数据";
			file_loss.writeError(it->first,str2,str3);

		}
	}
}

//函数声明
void DataSave(Message m);
//存储主题
string single_tp="";
vector<string>  range_tp;
vector<string>  multi_tp;
int flag=0;



map<string,Message> myMap;
/*//打印函数
void print(string str)
{
	map<string,Message>::iterator it;
	it=myMap.find(str);
	if(it!=myMap.end())
	{
		cout<<it->second.tpName<<"---"<<it->second.data<<"---"\
		<<it->second.time<<endl;
		DataSave(it->second);
	}else{cout<<str<<"插入map失败！"<<endl;}
}*/

void showMenu(vector<string> m)
{
	Menu* menu=new SubMenu;
	int n=0;
	int *p=NULL;
	int num;
	cin>>num;
	vector<int> v;
	map<string,Message>::iterator it;
	stringstream ss;
	string s;
	switch(num){
	case 1:
		n=menu->SubMenu1();
		ss<<n;
		s="主题"+ss.str();
		single_tp=s;
		ss.clear();
		ss.str("");
		break;
	case 2:
		p=menu->SubMenu2();
		if(p[0]< p[1])
		{
			for(int i=p[0];i<p[1]-p[0]+2;i++)
			{
				ss;
				ss<<i;
				s="主题"+ss.str();
				range_tp.push_back(s);
				ss.clear();
				ss.str("");
			}
			flag=1;
			if(p){
			delete []p;
			}
		}
		break;
	case 3:
		v=menu->SubMenu3();
		for(int j=0;j<v.size();j++)
		{
			ss;
			ss<<v[j];
			s="主题"+ss.str();
			multi_tp.push_back(s);
			ss.clear();
			ss.str("");
		}
		flag=2;
		break;
	case 4:
		return;
	default:
		break;
	} 
	return;
}
/*
void  DataSave(Message m1)
{
	file.writeLog(m1);
}*/
string getTime()
{
	char str[100]={};
	time_t t;
	time(&t);
	tm* p=localtime(&t);
	sprintf(str,"%4d/%d/%d-%d:%d:%d",p->tm_year+1900,\
	p->tm_mon+1,p->tm_mday,p->tm_hour,p->tm_min,p->tm_sec);
	string s=str;
	return s;
}
class StationMessageI:virtual public StationMessage{
public:
	virtual void getMsg(const Message& m,const Ice::Current&p)
	{
		sleep(20);
		map<string,int>::iterator iter;
		iter=flag_map.find(m.tpName);
		if(iter==flag_map.end())
		{
			flag_map.insert(pair<string,int>(m.tpName,m.flag));
		}else{
			iter->second+=1;
		}
		//测试循环订阅主题
		//cout<<"主题4订阅中"<<endl;
		Ice::ConnectionInfoPtr cip=p.con->getInfo();
		Ice::IPConnectionInfoPtr ip=Ice::IPConnectionInfoPtr::dynamicCast(cip);
		string str1,str2,str3;
		//判断消息是否丢失
		if(flag_map[m.tpName]!=m.flag)
		{
			stringstream h1,h2;
			h1<<ip->remoteAddress<<":"<<ip->remotePort;
			h2<<ip->localAddress<<":"<<ip->localPort;
			string str1=h1.str();
			string str2=h2.str();
			string str3=getTime();
			char logerror[100]={};
			sprintf(logerror,"error/error_%d.txt",ip->localPort);
			myFile file_error(logerror);
			file_error.writeError(str1,str2,str3);	
			h1.clear();
			h2.clear();
			h1.str("");
			h2.str("");
			return;
		}
		char logName[100]={};
		sprintf(logName,"log/log_%d.txt",ip->localPort);
		if (flag_map[m.tpName]>=1000) {
			flag_map[m.tpName]=0;
		}
		cout << "远端Ip：" << ip->remoteAddress << "---远端Port:" << ip->remotePort << endl;
		int data_size=m.tpName.length()+m.data.length()+m.time.length();
		cout<<"接收数据大小："<<data_size<<"字节"<<endl;
		cout<<m.tpName<<"---"<<m.data<<"---"<<m.time<<endl;
		myFile file(logName);
		file.writeLog(m);
	}
};

int main(int argc,char* argv[])
{
	Menu::MenuList();
	Ice::CommunicatorPtr ic;
	//读取配置文件
	Ice::InitializationData initdata;
	initdata.properties=Ice::createProperties();
	initdata.properties->load("Config/config.server");
	ic=Ice::initialize(initdata);
	try{
	Ice::ObjectPrx obj=ic->propertyToProxy("server.proxy");
	IceStorm::TopicManagerPrx topicManager=IceStorm::TopicManagerPrx::checkedCast(obj);
	Ice::ObjectAdapterPtr adapter=ic->createObjectAdapter("StationMessageAdapter");
	//直接初始化
	/*ic=Ice::initialize();

	Ice::ObjectPrx obj=ic->stringToProxy("StationTest/TopicManager:default -h localhost -p 9999");
	IceStorm::TopicManagerPrx topicManager=IceStorm::TopicManagerPrx::checkedCast(obj);
	Ice::ObjectAdapterPtr adapter=ic->createObjectAdapterWithEndpoints("StationMessageAdapter","default -h localhost -p 5566");*/
	

	//获取所有主题
	IceStorm::TopicDict topic_map=topicManager->retrieveAll();

	StationMessagePtr stm=new StationMessageI;
	Ice::ObjectPrx proxy=adapter->addWithUUID(stm);
	adapter->activate();
	
	IceStorm::TopicPrx topic;
	//遍历map
	IceStorm::TopicDict::iterator it;
	
	//从主题管理器获取主题列表，存在本地
	vector<string> v1;
	for(it=topic_map.begin();it!=topic_map.end();it++)
	{
		v1.push_back(it->first);
	}
	showMenu(v1);
	/*
	//测试--循环订阅单个主题
	IceStorm::QoS q;
	IceStorm::TopicPrx mytopic;
	while(true){
	mytopic=topicManager->retrieve("主题4");
	mytopic->subscribeAndGetPublisher(q,proxy);
	sleep(30);
	mytopic->unsubscribe(proxy);
	}*/
	
	//定时检测
	//signal(SIGALRM,signal_func);
	//set_timer();
	
	//1.订阅想要的主题;
	IceStorm::QoS q;
	IceStorm::TopicPrx mytopic;
	switch(flag){
	
	case 0:
		try{
			mytopic=topicManager->retrieve(single_tp);
			//cout<<"---"<<mytopic->getPublisher()<<endl;
			mytopic->subscribeAndGetPublisher(q,proxy);
		}catch(IceStorm::NoSuchTopic&e){cout<<e<<endl;}
		break;
	case 1:
		for(int i=0;i<range_tp.size();i++)
		{
			try{
				mytopic=topicManager->retrieve(range_tp[i]);
				//cout<<"---"<<mytopic->getPublisher()<<endl;
				mytopic->subscribeAndGetPublisher(q,proxy);
			}catch(IceStorm::NoSuchTopic&e){cout<<e<<endl;}
		}
		break;
		
	case 2:
		for(int j=0;j<multi_tp.size();j++)
		{
			try{
				mytopic=topicManager->retrieve(multi_tp[j]);
				mytopic->subscribeAndGetPublisher(q,proxy);
			}catch(IceStorm::NoSuchTopic&e){cout<<e<<endl;}
		}
		break;
	default:
		cout<<"请检查flag"<<endl;
		break;
	
	
	}
	}catch(Ice::TimeoutException&error){cout<<error<<endl;}
	//2.订阅所有主题
	/*for(it=topic_map.begin();it!=topic_map.end();it++)
	{
		try{
			IceStorm::QoS qos;
			it->second->subscribeAndGetPublisher(qos,proxy);
		}catch(IceStorm::NoSuchTopic&e)
		{
			cout<<e<<endl;
		}	
	}*/
	if(ic){
	ic->waitForShutdown();
	ic->destroy();
	}
}


