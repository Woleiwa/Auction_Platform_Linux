#include"../inc/admin.h"
#include"../inc/user.h"
#include"../inc/commodity.h"
#include"../inc/order.h"
#include"../inc/User_List.h"
#include<mutex>
#include<pthread.h>
#include"../inc/Order_List.h"
#include"../inc/Commodity_List.h"
#include <unistd.h>
//#define _TIMETEST_
//#define _USERTEST_
//#define _ADMINTEST_
#define _MAIN_
#ifdef _TIMETEST_
int main()
{
	Time mytime;
	mytime.current_time();
	string time = mytime.time_to_string();
	cout << time;
	my_time st = mytime.string_to_time(time);
	cout << st.year << '-' << st.month << '-' << st.day << ' ' << st.hour << ':' << st.minute << ':' << st.second;
}
#endif // _TIMETEST_

#ifdef _USERTEST_
int main()
{
	User_List ulist;
	ulist.read_from_txt();
	//ulist.user_register();
	ulist.sign_in();
}
#endif // _USERTEST_

#ifdef _ADMINTEST_
int main()
{
	Admin admin;
	admin.admin_operate();
}
#endif // _ADMINTEST_

#ifdef _MAIN_
void *start(void* arg);
void *update(void * arg);
int instruction_input(int mini, int max, string opline);
static const string operate_line = "instruction:1.user_login 2.user_register 3.admin_login 4.forget_password 5.exit";
mutex o_mtx;
mutex c_mtx;
mutex u_mtx;
bool update_flag = true;
User_List ulist;
Order_List olist;
Commodity_List clist;

int main()
{
	pthread_t tid1,tid2;
	int ret1 = pthread_create(&tid1,NULL,start,NULL);
	int ret2 = pthread_create(&tid2,NULL,update,NULL);
	pthread_join(tid1,NULL);
	pthread_join(tid2,NULL);
	pthread_exit(NULL);
}

void *start(void* arg)
{
	cout << endl;
	cout << "Welcome to my auction platform!" << endl;
	cout << "\033[33m";
	cout << "==============================================================================================================" << endl;
	cout << operate_line << endl;
	cout << "==============================================================================================================" << endl;
	cout << "\033[0m";
	cout << "Please input your instruction:";
	int judge = instruction_input(1, 5, operate_line);
	if (judge == 1)
	{
		
		ulist.read_from_txt();
		ulist.sign_in();
		start(NULL);
	}
	else if (judge == 2)
	{
		ulist.read_from_txt();
		ulist.user_register();
		start(NULL);
	}
	else if (judge == 3)
	{
		Admin admin;
		admin.admin_operate();
		start(NULL);
	}
	else if (judge == 4)
	{
		ulist.read_from_txt();
		ulist.forget_password();
		ulist.write_to_txt();
		start(NULL);
	}
	else
	{
		update_flag = false;
		cout << "******See you!******" << endl;
		
	}
}

void * update(void* arg)
{
	Commodity_List upclist;
	while (update_flag)
	{	
		upclist.read_from_txt();
		if (upclist.update())
		{
			upclist.write_to_txt();
			cout << endl<<"Information updated!" << endl;
		}
		
	}
}

int instruction_input(int mini, int max, string opline)
{
	int judge;
	cin >> judge;
	while (judge > max || judge < mini|| cin.fail())
	{
		while (cin.fail())
		{
			cin.clear();
			cin.ignore();
		}
		cout << endl;
		cout << "\033[31m";
		cout << "******Error input******" << endl;
		cout << "\033[33m";
		cout << "==============================================================================================================" << endl;
		cout << opline << endl;
		cout << "==============================================================================================================" << endl;
		cout << "\033[0m";
		cout << "Please input your instruction:";
		cin >> judge;
	}
	return judge;
}

#endif // _MAIN_
