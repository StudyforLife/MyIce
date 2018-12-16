#include"SubMenu.h"
#include"Menu.h"
#include<iostream>
#include<stdlib.h>
using namespace std;
int SubMenu::SubMenu1(){
	
	system("clear");
	cout<<"请输入你想要的主题（单个）："<<endl;
	int num;
	cin>>num;
	return num;
}
int* SubMenu::SubMenu2( ){
	
	system("clear");
	cout<<"请输入主题区间："<<endl;

	int num1,num2;
	cin>>num1>>num2;
	int* p = new int[2]; 
	*p= num1;
	*(p+1)=num2;
	return p;
}


vector<int>  SubMenu::SubMenu3(){

	system("clear");
	cout<<"输入a结束输入！"<<endl;
	cout<<"请输入主题号(多个)："<<endl;
	int temp;
	char c;
	while((cin>>temp).get(c))
	{
		if(c=='a')
			break;
		v.push_back(temp);
	}
	return v;
	
}

