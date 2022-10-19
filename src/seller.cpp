#include "../inc/seller.h"
#include "../inc/mytime.h"
#include "../inc/commodity.h"
#include "../inc/user.h"
#include <iomanip>
#include <unistd.h>
#include "../inc/order.h"
#include "../inc/Commodity_List.h"
#include "../inc/Order_List.h"
extern Order_List olist;
extern Commodity_List clist;
extern int instruction_input(int mini, int max, string opline);
static const string operate_line = "instruction:1.release 2.check my commodity 3.modify infomation 4.off_shelf 5.check order 6.re-onshelf \n7.exit";

Seller::~Seller()
{
}

void Seller::operate()
{
	
	cout << endl;
	cout << "******Welcome to Seller Platform******" << endl;
	cout << "\033[33m";
	cout << "==============================================================================================================" << endl;
	cout << operate_line << endl;
	cout << "==============================================================================================================" << endl;
	cout << "\033[0m";
	cout << "Please input:";
	int judge = instruction_input(1, 7, operate_line);
	switch (judge)
	{
	case 1:
		add_commodity();
		operate();
		break;
	case 2:
		check_commodity();
		operate();
		break;
	case 3:
		modify_commo_information();
		operate();
		break;
	case 4:
		off_shelf();
		operate();
		break;
	case 5:
		check_order();
		operate();
		break;
	case 6:
		re_on_shelf();
		operate();
		break;
	case 7:
		return;
		break;
	default:
		break;
	}
}

void Seller::add_commodity()
{
	string name;
	string description;
	inform my_inform = this->get_inform();

	cout << "Please input the name of your commodity:" << endl;
	while (name.length() == 0)
	{
		getline(cin, name);
	}

	cout << "Please input the description:" << endl;
	while (description.length() == 0)
	{
		getline(cin, description);
	}

	double price;
	double add_price;

	cout << "Please input the initial price of your commodity:" << endl;
	cin >> price;

	while (price < 0 || cin.fail())
	{
		while (cin.fail())
		{
			cin.ignore();
			cin.clear();
		}
		cout << "\033[31m";
		cout << "Please set an appropriate price." << endl;
		cout << "\033[0m";
		cin >> price;
	}
	cout << "Please input the minimum markup:" << endl;
	cin >> add_price;

	while (add_price < 0 || cin.fail())
	{
		while (cin.fail())
		{
			cin.ignore();
			cin.clear();
		}
		cout << "\033[31m";
		cout << "Please set an appropriate mark up." << endl;
		cout << "\033[0m";
		cin >> add_price;
	}
	cout << "Please input the number of your commodity:" << endl;
	int num;
	cin >> num;
	while (num <= 0 || cin.fail())
	{
		while (cin.fail())
		{
			cin.ignore();
			cin.clear();
		}
		cout << "\033[31m";
		cout << "Please set an appropriate number." << endl;
		cout << "\033[0m";
		cin >> num;
	}

	while (add_price < 0)
	{
		cout << "\033[31m";
		cout << "Please set an appropriate markup:" << endl;
		cout << "\033[0m";
		cin >> add_price;
	}

	string uid = my_inform.id;

	cout << "Please check the information, input 'yes' to add the commodity, and other to cancel:" << endl;
	string judge;
	cin >> judge;
	if (judge != "yes")
	{
		cout << endl
			 << "You have cancelled your releasion";

		

		return;
	}
	if (num > 1)
	{
		cout << "If you want to release seperately, please input 'yes', input other to release as a whole:";
		string judge;
		cin >> judge;
		if (judge != "yes")
		{
			Commodity mycommodity(name, price, add_price, num, description, uid);
			mycommodity.create_commodity();
		}
		else
		{
			while (num > 0)
			{
				Commodity mycommodity(name, price, add_price, 1, description, uid);
				mycommodity.create_commodity();
				num--;
			}
		}
	}
	else
	{
		Commodity mycommodity(name, price, add_price, num, description, uid);
		mycommodity.create_commodity();
	}
	cout << endl
		 << "You have released a commodity";

	
}

void Seller::check_commodity()
{
	clist.read_from_txt();
	commodity_list *head = clist.seller_check(this->get_inform().id);
	if (head == NULL)
	{
		cout << "\033[31m";
		cout << "You haven't released any commodity." << endl;
		cout << "\033[0m";

		

		return;
	}
	else
	{
		cout << left << setw(8) << "Id" << setw(30) << "Name" << setw(8) << "price" << setw(8) << "markup" << setw(8) << "number" << setw(24) << "releasing time" << setw(12) << "state";
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
				cout << "\033[33m";
				state = "Timeout";
			}
			cout << endl
				 << left << setw(8) << head->data.id << setw(30) << head->data.name << setw(8) << head->data.price << setw(8) << head->data.add_price
				 << setw(8) << head->data.num << setw(24) << str_time << setw(12) << state;
			head = head->next;
		}
		cout << "\033[0m";
	}

	
}

void Seller::check_order()
{
	olist.read_from_txt();
	string uid = this->get_inform().id;
	order_list *head = olist.seller_check(uid);
	if (head == NULL)
	{
		cout << "\033[31m";
		;
		cout << "You have no order." << endl;
		cout << "\033[0m";

		

		return;
	}
	cout << left << setw(8) << "Order" << setw(8) << "Auction" << setw(8) << "Commo_id" << setw(30) << "Commodity name" << setw(8) << "Bid" << setw(8) << "Number" << setw(24) << "Time" << setw(12) << "State" << endl;
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
			cout << "\033[35m";
		}
		Time time(head->data.time);
		string str_time = time.time_to_string();
		cout << left << setw(8) << head->data.order_id << setw(8) << head->data.auctioneer << setw(8) << head->data.commodity_id << setw(30) << head->data.commodity_name
			 << setw(8) << head->data.bid << setw(8) << head->data.num << setw(24) << str_time << setw(12) << state << endl;
		head = head->next;
	}
	cout << "\033[0m";

	
}

void Seller::modify_commo_information()
{
	clist.read_from_txt();
	commodity_list *head = clist.seller_check(this->get_inform().id);
	commodity_list *list = head;
	int num = 0;
	while (head)
	{
		if (head->data.st == OnSale)
		{
			cout << head->data.id << endl;
			num++;
		}
		head = head->next;
	}
	if (num == 0)
	{
		cout << "\033[31m";
		cout << "You don't have any commodity on sale now!" << endl;
		cout << "\033[0m";

		return;
	}
	cout << "Please input the commodity id you want to modify:" << endl;
	string commodity_id;
	cin >> commodity_id;
	head = list;
	bool flag = false;
	while (head)
	{
		if (head->data.st == OnSale)
		{
			if (head->data.id == commodity_id)
			{
				flag = true;
				break;
			}
		}
		head = head->next;
	}
	if (!flag)
	{
		cout << "\033[31m";
		cout << "No such commodity!";
		cout << "\033[0m";
		return;
		
	}
	Commodity cur_commo(commodity_id);
	commodity_inform info = cur_commo.my_info();
	cout << "Commodity id:" << info.id << endl;
	cout << "Commodity name:" << info.name << endl;
	cout << "Commodity price:" << info.price << endl;
	cout << "Commodity markup:" << info.add_price << endl;
	cout << "Commodity description:" << info.description << endl;
	Time time(info.auction_time);
	string str_time = time.time_to_string();
	cout << "On shelf time:" << str_time << endl;
	cout << "\033[33m";
	cout << "==================================================================================================================" << endl;
	cout << "Input 1 to modify name, 2 to modify description, others to exit" << endl;
	cout << "==================================================================================================================" << endl;
	cout << "\033[0m";
	string judge;
	cin >> judge;
	if (judge == "1" || judge == "2")
	{
		cout << "Please input your new information:" << endl;
		string information;
		while (information.length() == 0)
		{
			getline(cin, information);
		}
		cur_commo.modify_information(judge, information);
		cout << "Modified successfully";

		return;
	}
	else
	{
		cout << "Operation has been cancelled!.";

		return;
	}
}

void Seller::off_shelf()
{
	clist.read_from_txt();
	commodity_list *head = clist.seller_check(this->get_inform().id);
	if (head == NULL)
	{
		cout << "\033[31m";
		cout << "You haven't released any commodity." << endl;
		cout << "\033[0m";
	}
	else
	{
		commodity_list *list = head;
		int num = 0;
		while (list)
		{
			if (list->data.st == OnSale)
			{
				cout << list->data.id << endl;
				num++;
			}
			list = list->next;
		}
		if (num == 0)
		{
			cout << "\033[31m";
			;
			cout << "Your commodities have been sold or offshelved" << endl;
			cout << "\033[0m";

			

			return;
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
			cout << "\033[31m";
			;
			cout << "You didn't release such a commodity or the commodity has been sold or offshelved." << endl;
			cout << "\033[0m";
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

void Seller::re_on_shelf()
{
	clist.read_from_txt();
	commodity_list *head = clist.seller_check(this->get_inform().id);
	if (head == NULL)
	{
		cout << "\033[31m";
		;
		cout << "You haven't released any commodity." << endl;
		cout << "\033[0m";

		

		return;
	}
	commodity_list *list = head;
	int num = 0;
	while (head)
	{
		if ((head->data.st == OffShelf || head->data.st == Timeout))
		{
			cout << head->data.id << endl;
			num++;
		}
		head = head->next;
	}
	if (num == 0)
	{
		cout << "\033[31m";
		;
		cout << "Your have no offshelved commodity." << endl;
		cout << "\033[0m";
		
		return;
	}
	cout << "Please input the id of the commodity you want to re on shelf:" << endl;
	string id;
	cin >> id;
	bool flag = false;
	while (list)
	{
		if ((list->data.st == OffShelf || list->data.st == Timeout) && list->data.id == id)
		{
			flag = true;
			break;
		}
		list = list->next;
	}
	if (!flag)
	{
		cout << "\033[31m";
		;
		cout << "You didn't release such a commodity or the commodity is on sale or sold." << endl;
		cout << "\033[0m";
	}
	else
	{
		cout << "Input 'yes' to affirm:" << endl;
		string judge;
		cin >> judge;
		if (judge == "yes")
		{
			Commodity com(id);
			com.commodity_re_onshelf();
			cout << "You have re-onshelved an commodity:" << id << endl;
		}
		else
		{
			cout << "Operation has been cancelled!." << endl;
		}
	}

	

	return;
}
