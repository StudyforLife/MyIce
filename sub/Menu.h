#ifndef __MENU_H_
#define __MENU_H_

#include<vector>
using namespace std;

class Menu{

public:
	Menu();
	static void MenuList();
	virtual int SubMenu1()=0;
	virtual int* SubMenu2()=0;
	virtual vector<int> SubMenu3()=0;
	~Menu();
};





#endif
