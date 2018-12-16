#include"Menu.h"
#include<iostream>
#include<stdlib.h>
using namespace std;
Menu::Menu(){}
Menu::~Menu(){}

void Menu::MenuList()
{
	cout<<"------功能选择------"<<endl;
	cout<<"   1.订阅单个主题  "<<endl;
	cout<<"   2.订阅主题区间  "<<endl;
	cout<<"   3.订阅多个主题  "<<endl;
	cout<<"   4.退出  "<<endl;
	cout<<"请输入你的选择："<<endl;
}


