#include "../inc/admin.h"
#include "../inc/user.h"
#include "../inc/commodity.h"
#include "../inc/mail.h"
#include "../inc/mytime.h"
#include "../inc/order.h"
#include "../inc/User_List.h"
#include "../inc/Order_List.h"
#include "../inc/Commodity_List.h"
#include "../inc/Mail_List.h"
#include <stdlib.h>
#include <unistd.h>
using namespace std;
extern User_List ulist;
extern Order_List olist;
extern Commodity_List clist;
extern void confidential_input(char password[]);
extern int instruction_input(int mini, int max, string opline);
static const string operate_line = "instruction:1.check commodities 2.search commodities 3.check orders 4.check users 5.freeze users\n6.thaw users 7.offshelf commodities 8.commodity information 9.massages 10.log out";

void display_commodity(commodity_list *head)
{
	if (head == NULL)
	{
		cout << "\e[31m"
			 << "Users haven't released any commodity."
			 << "\e[0m" << endl;

		
	}
	else
	{
		cout << left << setw(8) << "Id" << setw(30) << "Name" << setw(8) << "price" << setw(8) << "markup" << setw(8) << "number" << setw(8) << "seller_id" << setw(24) << "releasing time" << setw(12) << "state";
		while (head)
		{
			Time time(head->data.auction_time);
			string str_time = time.time_to_string();
			string state = "OnSale";
			cout << "\033[32m";
			if (head->data.st == Sold)
			{
				state = "Sold";
				cout << "\033[34m";
			}
			else if (head->data.st == OffShelf)
			{
				cout << "\033[35m";
				state = "OffShelf";
			}
			else if (head->data.st == Timeout)
			{
				cout << "\033[36m";
				state = "Timeout";
			}
			cout << endl
				 << left << setw(8) << head->data.id << setw(30) << head->data.name << setw(8) << head->data.price << setw(8) << head->data.add_price
				 << setw(8) << head->data.num << setw(8) << head->data.user_id << setw(24) << str_time << setw(12) << state;
			head = head->next;
		}
		cout << "\033[0m";

		
	}
}

Admin::Admin()
{
	string id;
	cout << "Please input your Admin_id:";
	cin >> id;
	while (id != this->user_id)
	{
		cout << endl
			 << "Please input the right Admin_id:" << endl;
		cin >> id;
	}
	char password[20];
	cout << "Please input your password:" << endl;
	confidential_input(password);
	string pass = password;
	while (pass != this->password)
	{
		cout << "Please input the right password:" << endl;
		confidential_input(password);
		pass = password;
	}
	cout << endl;
}

void Admin::admin_operate()
{

	cout << endl;
	cout << "******Welcome to Admin platform******" << endl;
	cout << "\033[33m";
	cout << "==============================================================================================================" << endl;
	cout << operate_line << endl;
	cout << "==============================================================================================================" << endl;
	cout << "\033[0m";
	cout << "Please input:";
	int judge = instruction_input(1, 10, operate_line);
	switch (judge)
	{
	case 1:
		check_commodity();
		admin_operate();
		break;
	case 2:
		search();
		admin_operate();
		break;
	case 3:
		check_order();
		admin_operate();
		break;
	case 4:
		check_user();
		admin_operate();
		break;
	case 5:
		freeze_user();
		admin_operate();
		break;
	case 6:
		thaw_user();
		admin_operate();
		break;
	case 7:
		off_shelf();
		admin_operate();
		break;
	case 8:
		get_information();
		admin_operate();
		break;
	case 9:
		check_mail();
		admin_operate();
		break;
	case 10:
		return;
		break;
	default:
		break;
	}
}

void Admin::check_user()
{
	ulist.read_from_txt();
	inform_list *head = ulist.get_information();
	cout << left << setw(12) << "User_id" << setw(12) << "Name" << setw(12) << "Address"
		 << setw(12) << "Contact" << setw(16) << "Balance" << setw(24) << "register_time" << setw(12) << "State" << endl;
	cout << "==============================================================================================================" << endl;
	cout << "\033[33m";
	while (head)
	{
		Time time(head->data.register_time);
		string str_time = time.time_to_string();
		string state = "active";
		if (head->data.con == frozen)
		{
			state = "frozen";
			cout << "\033[34m";
		}
		cout << left << setw(12) << head->data.id << setw(12) << head->data.name << setw(12)
			 << head->data.address << setw(12) << head->data.contact << setw(16) << head->data.money << setw(24)
			 << str_time << setw(12) << state << endl;
		cout << "\033[33m";
		head = head->next;
	}

	

	cout << "\033[0m";
}

void Admin::check_commodity()
{
	clist.read_from_txt();
	commodity_list *head = clist.admin_check();
	display_commodity(head);
}

void Admin::check_order()
{
	olist.read_from_txt();
	order_list *head = olist.admin_check();
	if (head == NULL)
	{
		cout << "There is no order." << endl;

		

		return;
	}
	cout << left << setw(8) << "Order" << setw(8) << "Seller" << setw(8) << "Auction" << setw(8) << "Commo_id" << setw(30) << "Commodity name" << setw(8) << "Bid" << setw(8) << "Number" << setw(24) << "Time" << setw(12) << "State" << endl;
	while (head)
	{
		string state = "Waiting";
		cout << "\033[32m";
		if (head->data.st == Deal)
		{
			state = "Deal";
			cout << "\033[34m";
		}
		else if (head->data.st == Cancel)
		{
			state = "Cancel";
			cout << "\033[36m";
		}
		Time time(head->data.time);
		string str_time = time.time_to_string();
		cout << left << setw(8) << head->data.order_id << setw(8) << head->data.seller_id << setw(8) << head->data.auctioneer << setw(8) << head->data.commodity_id << setw(30) << head->data.commodity_name
			 << setw(8) << head->data.bid << setw(8) << head->data.num << setw(24) << str_time << setw(12) << state << endl;
		head = head->next;
	}
	cout << "\033[0m";

	
}

void Admin::off_shelf()
{
	clist.read_from_txt();
	commodity_list *head = clist.admin_check();
	if (head == NULL)
	{
		cout << "Users haven't released any commodity." << endl;
	}
	else
	{
		commodity_list *list = head;
		while (list)
		{
			if (list->data.st == OnSale)
			{
				cout << list->data.id << endl;
			}
			list = list->next;
		}
		cout << "Please input the id of the commodity you want to off_shelf:" << endl;
		string id;
		cin >> id;
		list = head;
		bool flag = false;
		while (list)
		{
			if (list->data.st == OnSale && list->data.id == id)
			{
				flag = true;
				break;
			}
			list = list->next;
		}
		if (!flag)
		{
			cout << "Users didn't release such a commodity or the commodity has been sold or offshelved." << endl;
		}
		else
		{
			cout << "Input 'yes' to affirm:" << endl;
			string judge;
			cin >> judge;
			if (judge == "yes")
			{
				Commodity com(id);
				com.offshelf();
				cout << "You have off shelved an commodity:" << id << endl;
			}
			else
			{
				cout << "Operation has been cancelled!." << endl;
			}
		}
	}

	

	return;
}

void Admin::freeze_user()
{
	ulist.read_from_txt();
	inform_list *head = ulist.get_information();
	inform_list *list = head;
	bool flag = false;
	while (head)
	{
		if (head->data.con == active)
		{
			cout << head->data.id << endl;
			flag = true;
		}
		head = head->next;
	}
	if (!flag)
	{
		cout << "There was no active users." << endl;

		

		return;
	}
	cout << "Please input the user id you want to freeze:" << endl;
	string uid;
	cin >> uid;
	flag = false;
	head = list;
	while (head)
	{
		if (head->data.id == uid && head->data.con == active)
		{
			flag = true;
			break;
		}
		head = head->next;
	}
	if (!flag)
	{
		cout << "No such user" << endl;

		

		return;
	}
	User user(uid);
	cout << "Input 'yes' to affirm:" << endl;
	string judge;
	cin >> judge;
	if (judge == "yes")
	{
		user.freeze();
		cout << "freeze successfully!" << endl;
	}
	else
	{
		cout << "Operation has been cancelled!" << endl;
	}

	

	return;
}

void Admin::thaw_user()
{
	ulist.read_from_txt();
	inform_list *head = ulist.get_information();
	inform_list *list = head;
	bool flag = false;
	while (head)
	{
		if (head->data.con == frozen)
		{
			cout << head->data.id << endl;
			flag = true;
		}
		head = head->next;
	}
	if (!flag)
	{
		cout << "There was no frozen users." << endl;

		

		return;
	}
	cout << "Please input the user id you want to thaw:" << endl;
	string uid;
	cin >> uid;
	flag = false;
	head = list;
	while (head)
	{
		if (head->data.id == uid && head->data.con == frozen)
		{
			flag = true;
			break;
		}
		head = head->next;
	}
	if (!flag)
	{
		cout << "No such user" << endl;

		

		return;
	}
	User user(uid);
	cout << "Input 'yes' to affirm:" << endl;
	string judge;
	cin >> judge;
	if (judge == "yes")
	{
		user.thaw();
		cout << "thaw successfully!" << endl;
	}
	else
	{
		cout << "Operation has been cancelled!" << endl;
	}

	

	return;
}

void Admin::get_information()
{
	cout << "Please input the commodity id you want to check:" << endl;
	string cid;
	cin >> cid;
	clist.read_from_txt();
	commodity_inform info = clist.admin_search_by_id(cid);
	if (strlen(info.id) == 0)
	{
		cout << "No such commodity!" << endl;

		

		return;
	}
	else
	{
		cout << "Commodity id:" << info.id << endl;
		cout << "Commodity name:" << info.name << endl;
		cout << "Commodity price:" << info.price << endl;
		cout << "Commodity markup:" << info.add_price << endl;
		cout << "Commodity description:" << info.description << endl;
		cout << "Commodity number:" << info.num << endl;
		Time time(info.auction_time);
		string str_time = time.time_to_string();
		cout << "On shelf time:" << str_time << endl;
		string state = "OnSale";
		if (info.st == Sold)
		{
			state = "Sold";
		}
		else if (info.st == OffShelf)
		{
			state = "OffShelf";
		}
		else if (info.st == Timeout)
		{
			state = "Timeout";
		}
		cout << "State:" << state << endl;
	}

	
}

extern void sort(commodity_list *head, int judge);

void Admin::search()
{
	cout << "Please input your key words:" << endl;
	string key_words;
	while (key_words.length() == 0)
	{
		getline(cin, key_words);
	}
	commodity_list *head = NULL;
	clist.read_from_txt();
	head = clist.admin_search_by_key_word(key_words);
	cout << "Sort by relevance to input 1, sort by time to input 2, sort by price to input 3, sort by number to input 4" << endl;
	int j;
	cin >> j;
	if (j == 1)
	{
		head = head;
	}
	else
	{
		sort(head, j);
	}
	display_commodity(head);
}

void Admin::check_mail()
{
	contacter *head = NULL;
	ulist.read_from_txt();
	string uid = "Admin";
	head = ulist.my_contacter();
	contacter *list = head;
	while (list)
	{
		cout << list->uid << endl;
		list = list->next;
	}
	cout << "Please input the people you want to send massage to:" << endl;
	string receiver;
	cin >> receiver;
	bool flag = false;

	list = head;
	while (list)
	{
		if (list->uid == receiver)
		{
			flag = true;
			break;
		}
		list = list->next;
	}
	if (!flag)
	{
		cout << "\033[32m";
		cout << "No such user!" << endl;
		cout << "\033[0m";

		return;
	}
	while (flag)
	{
		Mail_List mlist(uid, receiver);
		mlist.read_from_txt();
		mail_list *mhead = mlist.get_mails();
		system("clear");
		sleep(1);
		while (mhead)
		{
			Mail cur_mail(uid, receiver, mhead->data);
			cur_mail.print_mail(uid);
			mhead = mhead->next;
		}
		mlist.read_mails(uid);
		mlist.write_to_txt();

		cout << endl
			 << "If you want to send a massage, please input 'yes':" << endl;
		string judge;
		cin >> judge;
		if (judge == "yes")
		{
			string content;
			cout << "Please input your massage:" << endl;
			while (content.length() == 0)
			{
				getline(cin, content);
			}
			Mail new_mail(uid, receiver, content);
			new_mail.add_to_list();
		}
		else
		{
			flag = false;
		}
	}

	return;
}
