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

	bool read_from_txt();//��ȡ��Ϣ

	void write_to_txt();//д����Ϣ

	void sign_in();//��¼

	void user_register();//�û�ע��

	inform_list* get_information();//��ȡ�����û���Ϣ

	inform_list* find_one_user(string uid);

	void forget_password();

	void update(string seller_id, string auctioneer_id, double money);

	contacter* my_contacter(string uid);

	contacter* my_contacter();
private:
	inform_list* user_head = NULL, * user_tail = NULL;

};

#endif // !_USERLIST_


