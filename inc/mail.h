#pragma once
#ifndef _MAIL_
#define _MAIL_
#include<string>
#include"mytime.h"

enum mail_state
{
	unRead, Read
};

struct mail
{
	bool direction;
	char massage[512] = "\0";
	my_time time;
	mail_state state = unRead;
};

struct mail_list
{
	mail data;
	mail_list* next = NULL;
};

class Mail
{
private:
	string u1, u2;
	mail data;
public:
	Mail(string u1, string u2, string content);

	void add_to_list();

	void print_mail(string uid);

	Mail(string sender, string receiver, mail data);
};

#endif // !_MAIL_


