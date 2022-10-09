#include "../inc/Mail_List.h"
#include<fstream>
#include"../inc/mytime.h"
Mail_List::Mail_List(string id1, string id2)
{
	if (strcmp(id1.c_str(), id2.c_str()) < 0)
	{
		this->u1 = id1;
		this->u2 = id2;
	}
	else
	{
		this->u1 = id2;
		this->u2 = id1;
	}
}

Mail_List::~Mail_List()
{
	while (head)
	{
		mail_list* cur = head;
		head = head->next;
		delete cur;
	}
	tail = NULL;
}

mail string_to_info(string st)
{
	int head = 0;
	int rear = 0;
	rear = st.find(',', head);
	string direction(st, head, rear - head);

	head = rear + 1;
	rear = st.find(',', head);
	string str_time(st, head, rear - head);

	head = rear + 1;
	rear = st.find(',', head);
	string state(st, head, rear - head);

	head = rear + 1;
	rear = st.length();
	string content(st, head, rear - head);

	
	mail res;
	strcpy(res.massage, content.c_str());

	Time time;
	my_time res_time = time.string_to_time(str_time);
	res.time = res_time;

	if (direction == "true")
	{
		res.direction = true;
	}
	else
	{
		res.direction = false;
	}

	if (state == "Read")
	{
		res.state = Read;
	}
	else
	{
		res.state = unRead;
	}
	return res;
}

string info_to_string(mail minfo) 
{
	string content = minfo.massage;
	string direction = "false";
	if (minfo.direction)
	{
		direction = "true";
	}
	string str_time;
	string state = "Read";
	if (minfo.state == unRead)
	{
		state = "unRead";
	}
	Time time(minfo.time);
	str_time = time.time_to_string();
	string res = direction + "," + str_time + "," + state + "," + content;
	return res;
}

bool Mail_List::read_from_txt()
{
	mail_list* cur = this->head;
	while (cur)
	{
		mail_list* temp = cur;
		cur = cur->next;
		delete temp;
	}
	this->head = NULL;
	this->tail = NULL;
	char buffer[256];
	string txt_name = this->u1 + "-" + this->u2 + ".txt";
	ifstream mailfile(txt_name);
	if (!mailfile.is_open())
	{
		return false;
	}

	while (!mailfile.eof())
	{
		mailfile.getline(buffer, 255);
		if (strlen(buffer) <= 1)
		{
			break;
		}

		string info(buffer);
		mail user_info = string_to_info(info);
		if (head == NULL)
		{
			head = new mail_list;
			tail = head;
		}
		else
		{
			tail->next = new mail_list;
			tail = tail->next;
		}
		tail->data = user_info;
		tail->next = NULL;
	}
	return true;
}

void Mail_List::write_to_txt()
{
	string txt_name = this->u1 + "-" + this->u2 + ".txt";
	ofstream mailfile(txt_name);
	if (!mailfile.is_open())
	{
		return;
	}
	else
	{
		mail_list* head = this->head;
		while (head != NULL)
		{
			string info_str = info_to_string(head->data);
			mailfile << info_str + '\n';
			head = head->next;
		}
	}
}

void Mail_List::add_mail(mail new_mail)
{
	if (this->head == NULL)
	{
		this->head = new mail_list;
		this->tail = this->head;
	}
	else
	{
		this->tail->next = new mail_list;
		this->tail = this->tail->next;
	}
	tail->data = new_mail;
	tail->next = NULL;
}

mail_list* Mail_List::get_mails()
{
	return this->head;
}

void Mail_List::read_mails(string uid)
{
	bool reader = !(uid == this->u1);
	mail_list* cur = this->head;
	while (cur)
	{
		if (cur->data.direction == reader )
		{
			cur->data.state = Read;
		}
		cur = cur->next;
	}
}
