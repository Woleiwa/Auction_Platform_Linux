#pragma once
#include"mail.h"

#ifndef _MAILLIST_
#define _MAILLIST_
class Mail_List
{
private: 
	mail_list* head = NULL, *tail;
	string u1, u2;                                     
public:
	Mail_List(string id1, string id2);

	~Mail_List();

	bool read_from_txt();

	void write_to_txt();

	void add_mail(mail new_mail);

	mail_list* get_mails();
	
	void read_mails(string uid);
};
#endif // !_MAILLIST_



