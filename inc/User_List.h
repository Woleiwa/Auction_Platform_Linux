#pragma once
#include"user.h"
#ifndef _USERLIST_
#define _USERLIST_

struct contacter
{
	string uid;
	contacter* next;
};

class User_List
{
public:
	User_List();

	~User_List();

	bool read_from_txt();//读取信息

	void write_to_txt();//写入信息

	void sign_in();//登录

	void user_register();//用户注册

	inform_list* get_information();//获取所有用户信息

	inform_list* find_one_user(string uid);

	void forget_password();

	void update(string seller_id, string auctioneer_id, double money);

	contacter* my_contacter(string uid);

	contacter* my_contacter();
private:
	inform_list* user_head = NULL, * user_tail = NULL;

};

#endif // !_USERLIST_


