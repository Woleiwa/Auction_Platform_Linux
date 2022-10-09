#include<cstdlib>
#include"../inc/User_List.h"
#include<unistd.h>
#include"../inc/user.h"
#include"../inc/User_List.h"
#include"../inc/mail.h"
#include"../inc/md5.h"
#include"../inc/mytime.h"
#include"../inc/order.h"
#include"../inc/commodity.h"
#include"../inc/Commodity_List.h"
#include"../inc/seller.h"
#include"../inc/consumer.h"
#include"../inc/Order_List.h"
#include<mutex>
extern mutex u_mtx;
extern User_List ulist;
extern Order_List olist;
extern Commodity_List clist;
#include <termio.h>


void confidential_input(char password[])
{
	string res  = getpass("\0");
	strcpy(password,res.c_str());
}

void string_to_md5(string md5, inform& res)
{
	int head = 0;
	int rear = 0;
	rear = md5.find(' ', head);
	string num1(md5, head, rear - head);

	head = rear + 1;
	rear = md5.find(' ', head);
	string num2(md5, head, rear - head);

	head = rear + 1;
	rear = md5.find(' ', head);
	string num3(md5, head, rear - head);

	head = rear + 1;
	rear = md5.length();
	string num4(md5, head, rear - head);

	res.md5_code[0] = (unsigned)stoll(num1);
	res.md5_code[1] = (unsigned)stoll(num2);
	res.md5_code[2] = (unsigned)stoll(num3);
	res.md5_code[3] = (unsigned)stoll(num4);
}

string md5_to_string(inform info)
{
	string num1 = to_string(info.md5_code[0]);
	string num2 = to_string(info.md5_code[1]);
	string num3 = to_string(info.md5_code[2]);
	string num4 = to_string(info.md5_code[3]);
	string res = num1 + ' ' + num2 + ' ' + num3 + ' ' + num4;
	return res;
}

static inform string_to_info(string st)
{
	int head = 0;
	int rear = 0;
	rear = st.find(',', head);
	string uid(st, head, rear - head);

	head = rear + 1;
	rear = st.find(',', head);
	string name(st, head, rear - head);

	head = rear + 1;
	rear = st.find(',', head);
	string md5_code(st, head, rear - head);

	head = rear + 1;
	rear = st.find(',', head);
	string address(st, head, rear - head);

	head = rear + 1;
	rear = st.find(',', head);
	string contact(st, head, rear - head);

	head = rear + 1;
	rear = st.find(',', head);
	string money(st, head, rear - head);

	head = rear + 1;
	rear = st.find(',', head);
	string register_time(st, head, rear - head);

	head = rear + 1;
	rear = st.length();
	string state(st, head, rear - head);

	inform res;
	strcpy(res.id, uid.c_str());
	strcpy(res.name, name.c_str());
	strcpy(res.address, address.c_str());
	strcpy(res.contact, contact.c_str());

	res.money = stod(money);
	Time time;
	res.register_time = time.string_to_time(register_time);
	string_to_md5(md5_code, res);

	if (state == "active")
	{
		res.con = active;
	}
	else
	{
		res.con = frozen;
	}

	return res;
}

static string info_to_string(inform info)
{
	string id = info.id;
	string name = info.name;
	string address = info.address;
	string contact = info.contact;
	Time register_time(info.register_time);
	string time = register_time.time_to_string();
	string money = to_string(info.money);
	string md5 = md5_to_string(info);
	string state;
	if (info.con == active)
	{
		state = "active";
	}
	else
	{
		state = "frozen";
	}
	string res = id + ',' + name + ',' + md5 + ',' + address + ',' + contact + ',' + money + ',' + time + ',' + state;
	return res;
}

User_List::User_List()
{

}

User_List::~User_List()
{
	while (this->user_head)
	{
		inform_list* list = this->user_head;
		this->user_head = list->next;
		delete list;
	}
}

bool User_List::read_from_txt()
{
	u_mtx.lock();
	inform_list* cur = this->user_head;
	while (cur)
	{
		inform_list* temp = cur;
		cur = cur->next;
		delete temp;
	}
	this->user_head = NULL;
	this->user_tail = NULL;
	char buffer[256];
	ifstream userfile("user.txt");
	if (!userfile.is_open())
	{
		u_mtx.unlock();
		return false;
	}

	while (!userfile.eof())
	{
		userfile.getline(buffer, 255);
		if (strlen(buffer) <= 1)
		{
			break;
		}

		string info(buffer);
		inform user_info = string_to_info(info);
		if (user_head == NULL)
		{
			user_head = new inform_list;
			user_head->data = user_info;
			user_head->next = NULL;
			user_tail = user_head;
		}
		else
		{
			user_tail->next = new inform_list;
			user_tail = user_tail->next;
			user_tail->data = user_info;
			user_tail->next = NULL;
		}
	}
	u_mtx.unlock();
	return true;
}

void User_List::write_to_txt()
{
	u_mtx.lock();
	ofstream userfile("user.txt");
	if (!userfile.is_open())
	{
		u_mtx.unlock();
		return;
	}
	else
	{
		inform_list* head = user_head;
		while (head != NULL)
		{
			string info_str = info_to_string(head->data);
			userfile << info_str + '\n';
			head = head->next;
		}
	}
	u_mtx.unlock();
}

bool Judge(char password[20])
{

	int length = strlen(password);

	if (length < 9)
	{
		return false;
	}

	bool j1 = false;
	bool j2 = false;
	bool j3 = false;

	for (int i = 0; i < length; i++)
	{
		if (password[i] - '0' <= 9 && password[i] - '0' >= 0)
		{
			j1 = true;
		}
		else if (password[i] - 'a' <= 25 && password[i] - 'a' >= 0)
		{
			j2 = true;
		}
		else if (password[i] - 'A' <= 25 && password[i] - 'A' >= 0)
		{
			j3 = true;
		}
	}
	return(j1 & j2 & j3);
}

void User_List::user_register()
{
	string name;
	cout << "Please input your user_name:";
	while (name.length() == 0)
	{
		getline(cin, name);
	}
	bool flag = false;

	inform_list* head = user_head;
	while (head)
	{
		if (head->data.name == name)
		{
			cout << "Repeated name! Please choose a new one:";
			while (name.length() == 0)
			{
				getline(cin, name);
			}
			head = user_head;
		}
		else
		{
			head = head->next;
		}
	}

	char password[20] = "\0";
	cout << "Please input your password(number,letter and captial required, no shorter than 9 characters):" << endl;

	while (strlen(password)== 0)
	{
		cin.getline(password, 19);
	}
	

	while (!Judge(password))
	{
		cout << "\033[31m";
		cout << "Please input an appropriate password:" << endl;
		cout << "\033[0m";
		while (strlen(password) == 0)
		{
			cin.getline(password, 19);
		}
	}

	unsigned int* md5 = MD5_2(password);

	string address;
	cout << "Please input your address:" << endl;
	while (address.length() == 0)
	{
		getline(cin, address);
	}

	string contact;
	cout << "Please input your phone_number or e_mail:" << endl;
	while (contact.length() == 0)
	{
		getline(cin, contact);
	}

	double money;
	cout << "Please set your initial account:" << endl;
	cin >> money;
	while (money < 0)
	{
		cout << "Please set a correct initial account:" << endl;
		cin >> money;
	}

	inform new_account;

	if (user_tail == NULL)
	{
		strcpy(new_account.id, "U00001");
	}
	else
	{
		string uid = user_tail->data.id;
		string unum(uid, 1, 5);
		int num = stoi(unum);
		num += 1;
		unum = to_string(num);
		while (unum.length() < 5)
		{
			unum = '0' + unum;
		}
		unum = 'U' + unum;
		strcpy(new_account.id, unum.c_str());
	}

	strcpy(new_account.name, name.c_str());
	new_account.md5_code[0] = md5[0];
	new_account.md5_code[1] = md5[1];
	new_account.md5_code[2] = md5[2];
	new_account.md5_code[3] = md5[3];

	strcpy(new_account.contact, contact.c_str());
	strcpy(new_account.address, address.c_str());

	Time cur;
	new_account.register_time = cur.current_time();

	new_account.money = money;

	new_account.con = active;

	if (user_tail == NULL)
	{
		user_tail = new inform_list;
		user_tail->data = new_account;
		user_tail->next = NULL;
		user_head = user_tail;
	}
	else
	{
		user_tail->next = new inform_list;
		user_tail = user_tail->next;
		user_tail->data = new_account;
		user_tail->next = NULL;
	}
	write_to_txt();
	cout << "\033[33m";
	cout << "You have registered an account! Your user id is :" << new_account.id << endl;
	cout << "\033[0m";
	
}

void User_List::sign_in()
{

	inform_list* head = user_head;
	if (!this->read_from_txt())
	{
		cout<<"\033[31mNo user now!Please register one!\033[0m";		
		return;
	}
	string id;
	cout << "Please input your user_id:" << endl;
	cin >> id;
	while (head)
	{
		if (head->data.id == id)
		{
			if (head->data.con == frozen)
			{
				cout << "\033[31m";
				cout << "Your account has been frozen!" << endl;
				cout << "\033[0m";
				
				return;
			}
			break;
		}
		else
		{
			head = head->next;
		}
		if (head == NULL)
		{
			cout << "\033[31m";
			cout << "Please input a correct user_id:";
			cout << "\033[0m";
			cin >> id;
			head = user_head;
		}
	}

	char password[20];
	cout << "Please input your password:" << endl;
	confidential_input(password);
	unsigned* md5 = MD5_2(password);
	bool flag = false;
	for (int i = 0; i < 5; i++)
	{
		if (md5[0] == head->data.md5_code[0] && md5[1] == head->data.md5_code[1]
			&& md5[2] == head->data.md5_code[2] && md5[3] == head->data.md5_code[3])
		{
			flag = true;
			cout << password << endl;
			break;
		}
		else
		{
			cout << "\033[31m";
			cout << endl << "Please input the right password(You have " << 5 - i << " time left):" << endl;
			cout << "\033[0m";
			md5 = MD5_2(password);
		}
	}
	//cout <<"flag == true" << endl;
	if (flag == true)
	{
		cout << endl << "You have logged in!";
		
		User user(id);
		user.operate();
	}
	else
	{
		User user(id);
		user.freeze();
		cout << "\033[31m";
		cout << "You have been frozen!" << endl;
		cout << "\033[0m";
		
		return;
	}
}

inform_list* User_List::get_information()
{
	return user_head;
}

inform_list* User_List::find_one_user(string uid)
{
	inform_list* head = this->user_head;
	while (head)
	{
		if (head->data.id == uid)
		{
			return head;
		}
		head = head->next;
	}
	return head;
}

void User_List::forget_password()
{
	cout << "Pleas input your account:" << endl;
	string uid;
	cin >> uid;
	inform_list* list = this->user_head;
	while (list)
	{
		if (list->data.id == uid && list->data.con == active)
		{
			cout << "Pleas input your contact:" << endl;
			string contact;
			cin >> contact;
			if (list->data.contact == contact)
			{
				char password[20] = "\0";
				cout << "Please input your new password(number,letter and captial required, no shorter than 9 characters):" << endl;
				while(strlen(password) ==0)
					cin.getline(password, 19);

				while (!Judge(password))
				{
					cout << "\033[31m";
					cout << "Please input an appropriate password:" << endl;
					cout << "\033[0m";
					cin.getline(password, 19);
				}

				unsigned int* md5 = MD5_2(password);

				list->data.md5_code[0] = md5[0];
				list->data.md5_code[1] = md5[1];
				list->data.md5_code[2] = md5[2];
				list->data.md5_code[3] = md5[3];

			}
			else
			{
				cout << "\033[31m";
				cout << "It doesn't match your information!" << endl;
				cout << "\033[0m";
				
				return;
			}
		}
		list = list->next;
	}
	cout << "\033[31m";
	cout << "No such user or your account has been frozen." << endl;
	cout << "\033[0m";
	
}

void User_List::update(string seller_id, string auctioneer_id, double money)
{
	inform_list* head = this->user_head;
	bool flag1 = false, flag2 = false;
	while (head && !(flag1 && flag2))
	{
		if (head->data.id == seller_id)
		{
			head->data.money += money;
			flag1 = true;
		}
		else if (head->data.id == auctioneer_id)
		{
			head->data.money -= money;
			flag2 = true;
		}
		head = head->next;
	}
}

contacter* User_List::my_contacter(string uid)
{
	inform_list* head = this->user_head;
	contacter* res_head = NULL, * res_tail = NULL;
	while (head)
	{
		if (head->data.id != uid)
		{
			if (res_head == NULL)
			{
				res_head = new contacter;
				res_tail = res_head;
			}
			else
			{
				res_tail->next = new contacter;
				res_tail = res_tail->next;
			}
			res_tail->uid = head->data.id;
			res_tail->next = NULL;
		}
		head = head->next;
	}
	return res_head;
}

contacter* User_List::my_contacter()
{
	inform_list* head = this->user_head;
	contacter* res_head = NULL, * res_tail = NULL;
	while (head)
	{
		if (res_head == NULL)
		{
			res_head = new contacter;
			res_tail = res_head;
		}
		else
		{
			res_tail->next = new contacter;
			res_tail = res_tail->next;
		}
		res_tail->uid = head->data.id;
		res_tail->next = NULL;
		head = head->next;
	}
	return res_head;
}
