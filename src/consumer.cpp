#include"../inc/consumer.h"
#include"../inc/commodity.h"
#include"../inc/Commodity_List.h"
#include"../inc/Order_List.h"
#include<unistd.h>
#include<vector>

extern Order_List olist;
extern Commodity_List clist;
extern int instruction_input(int mini, int max, string opline);
static const string operate_line = "instruction:1.auction 2.check all commodity 3.search 4.check order 5.get information 6.cancel order 7.exit" ;

void Consumer::operate()
{
	cout << endl;
	cout << "******Welcome to Consumer Platform******" << endl;
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
		auction();
		operate();
		break;
	case 2:
		check_commodity();
		operate();
		break;
	case 3:
		search();
		operate();
		break;
	case 4:
		check_order();
		operate();
		break;
	case 5:
		get_information();
		operate();
		break;
	case 6:
		cancel_order();
		operate();
		break;
	case 7:
		return;
		break;
	default:
		break;
	}
}

void Consumer::check_commodity()
{
	clist.read_from_txt();
	commodity_list* head = clist.consumer_check(this->get_inform().id);
	if (head == NULL)
	{
		cout << "\033[31m";
		cout << "There were no commodities for you." << endl;
		cout << "\033[0m";
		
		return;
	}
	else
	{
		cout << left << setw(8) << "Id" << setw(30) << "Name" << setw(8) << "price" << setw(8) << "markup" << setw(8) << "number" << setw(12) << "seller_id" << setw(24) << "releasing time" << setw(12) << "state";
		cout << "\032[0m";
		while (head)
		{
			Time time(head->data.auction_time);
			string str_time = time.time_to_string();
			string state = "OnSale";
			cout << endl << left << setw(8) << head->data.id << setw(30) << head->data.name << setw(8) << head->data.price << setw(8) << head->data.add_price
				<< setw(8) << head->data.num << setw(12) << head->data.user_id << setw(24) << str_time << setw(12) << state;
			head = head->next;
		}
		cout << "\033[0m";
		string judge;
		cout << endl << "Do you want to auction? Input 'yes' to auction."<<endl;
		cin >> judge;
		if (judge != "yes")
		{
			
			return;
		}
		else
		{
			cout << endl << "Please input the commodity id you want to auction." << endl;
			string c_id;
			cin >> c_id;
			auction(c_id);
		}
	}
}

void Consumer::check_order()
{
	olist.read_from_txt();
	string uid = this->get_inform().id;
	order_list* head = olist.consumer_check(uid);
	if (head == NULL)
	{

		cout << "\033[31m";
		cout << "You have no order." << endl;
		cout << "\033[0m";
		
		return;
	}
	cout << left << setw(8) << "Order" << setw(8) << "Seller" << setw(8) << "Commo_id" << setw(30) << "Commodity name" << setw(8) << "Bid" << setw(8) <<"Number" << setw(24) << "Time" << setw(12) << "State" << endl;
	while (head)
	{
		string state = "Waiting";
		cout << "\032[0m";
		if (head->data.st == Deal)
		{
			state = "Deal";
			cout << "\033[34m";
		}
		else if (head->data.st == Cancel)
		{
			cout << "\033[35m";
			state = "Cancel";
		}
		Time time(head->data.time);
		string str_time = time.time_to_string();
		cout << left << setw(8) << head->data.order_id << setw(8) << head->data.seller_id << setw(8) << head->data.commodity_id << setw(30) << head->data.commodity_name
			<< setw(8) << head->data.bid << setw(8) << head->data.num << setw(24) << str_time << setw(12) << state << endl;
		head = head->next;
	}
	cout << "\033[0m";
	
}

void Consumer::auction()
{
	cout << endl << "Please input the commodity id you want to auction." << endl;
	string c_id;
	cin >> c_id;
	auction(c_id);
}

void Consumer::auction(string id)
{
	Commodity commo(id);
	commodity_inform commo_info = commo.my_info();
	if (strlen(commo_info.id) == 0 || commo_info.st != OnSale)
	{
		cout << "No such commodity!" << endl;
	}
	else
	{
		olist.read_from_txt();
		double mininum_price = 0;
		mininum_price = olist.max_price(id);
		cout << "The highest bid now is:" << mininum_price << endl;
		cout << "Its markup is:" << commo_info.add_price << endl;
		cout << "Please input your bid:" << endl;
		double offer;
		cin >> offer;
		while (offer < mininum_price + commo_info.add_price || offer > this->get_inform().money || cin.fail())
		{
			while (cin.fail())
			{
				cin.ignore();
				cin.clear();
			}
			cout << "Please input an appropriate bid(maybe you don't have enough money):" << endl;
			cin >> offer;
		}

		int num;
		cout << "Please input the number you want to bid for:" << endl;
		cin >> num;
		while (num <= 0|| num >commo_info.num || num * offer > this->get_inform().money || cin.fail())
		{
			while (cin.fail())
			{
				cin.ignore();
				cin.clear();
			}
			cout << "Please input an appropriate number:" << endl;
			cin >> num;
		}
		string seller_id = commo_info.user_id;
		string auctioneer_id = this->get_inform().id;
		string commo_id = commo_info.id;
		string name = commo_info.name;
		Order my_order(seller_id, auctioneer_id,commo_id, name, offer, num);
		cout << "Input 'yes' to affirm:" << endl;
		string judge;
		cin >> judge;
		if (judge == "yes")
		{
			my_order.add_to_olist();
			cout << "You have made an order!" << endl;
		}
		else
		{
			cout << "Operation has been cancelled!!" << endl;
		}
	}
	
	return;
}

void Consumer::get_information()
{
	cout << "Please input the commodity id you want to check:" << endl;
	string cid;
	cin >> cid;
	clist.read_from_txt();
	commodity_inform info = clist.consumer_search_by_id(cid);
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
		cout << "Commodity number:" << info.num << endl;
		cout << "Commodity description:" << info.description << endl;
		Time time(info.auction_time);
		string str_time = time.time_to_string();
		cout << "On shelf time:" << str_time << endl;
		cout << "If you want to auction this commodity, please input 'yes':" << endl;
		string judge;
		cin >> judge;
		if (judge == "yes")
		{
			this->auction(cid);
		}
	}
}

void merge_sort_by_time(vector<commodity_inform>& c_list, int start, int end)
{
	if(start >= end - 1)
	{
		return;
	}
	int mid = (start + end )/2;
	merge_sort_by_time(c_list,start, mid);
	merge_sort_by_time(c_list,mid + 1, end);
	vector<commodity_inform> c_list_ordered;
	int i = start;
	int j = mid + 1;
	while(i <= mid && j <= end )
	{
		Time time1 (c_list[i].auction_time);
		Time time2 (c_list[j].auction_time);
		if(time1 < time2)
		{
			c_list_ordered.push_back(c_list[i]);
			i++;
		}
		else
		{
			c_list_ordered.push_back(c_list[j]);
			j++;
		}
		
	}
	while (i <= mid)
	{
		c_list_ordered.push_back(c_list[i]);
		i++;
	}
	while(j <= end)
	{
			c_list_ordered.push_back(c_list[j]);
			j++;
	}
	for(int i = 0; i < end - start + 1; i++)
	{
		c_list[i+ start] = c_list_ordered[i];
	}
}

void merge_sort_by_price(vector<commodity_inform>& c_list, int start, int end)
{
	if(start >= end - 1)
	{
		return;
	}
	int mid = (start + end )/2;
	merge_sort_by_price(c_list,start, mid);
	merge_sort_by_price(c_list,mid + 1, end);
	vector<commodity_inform> c_list_ordered;
	int i = start;
	int j = mid + 1;
	while(i <= mid && j <= end )
	{
		if(c_list[i].price < c_list[j].price)
		{
			c_list_ordered.push_back(c_list[i]);
			i++;
		}
		else
		{
			c_list_ordered.push_back(c_list[j]);
			j++;
		}
		
	}
	while (i <= mid)
	{
		c_list_ordered.push_back(c_list[i]);
		i++;
	}
	while(j <= end)
	{
			c_list_ordered.push_back(c_list[j]);
			j++;
	}
	for(int i = 0; i < end - start + 1; i++)
	{
		c_list[i+ start] = c_list_ordered[i];
	}
}

void merge_sort_by_number(vector<commodity_inform>& c_list, int start, int end)
{
	if(start >= end - 1)
	{
		return;
	}
	int mid = (start + end )/2;
	merge_sort_by_number(c_list,start, mid);
	merge_sort_by_number(c_list,mid + 1, end);
	vector<commodity_inform> c_list_ordered;
	int i = start;
	int j = mid + 1;
	while(i <= mid && j <= end )
	{
		if(c_list[i].num < c_list[j].num)
		{
			c_list_ordered.push_back(c_list[i]);
			i++;
		}
		else
		{
			c_list_ordered.push_back(c_list[j]);
			j++;
		}
		
	}
	while (i <= mid)
	{
		c_list_ordered.push_back(c_list[i]);
		i++;
	}
	while(j <= end)
	{
			c_list_ordered.push_back(c_list[j]);
			j++;
	}
	for(int i = 0; i < end - start + 1; i++)
	{
		c_list[i+ start] = c_list_ordered[i];
	}
}

void sort(commodity_list* head, int judge)
{
	commodity_list* list = head;
	vector<commodity_inform> c_list;
	while (list)
	{
		c_list.push_back(list->data);
		list = list->next;
	}
	if(judge == 2)
	{
		merge_sort_by_time(c_list, 0, c_list.size() - 1);
	}
	else if(judge == 3)
	{
		merge_sort_by_price(c_list, 0, c_list.size() - 1);
	}
	else if(judge == 4)
	{
		merge_sort_by_number(c_list, 0, c_list.size() - 1);
	}
	int i = 0;
	list = head;
	while (list)
	{
		list->data = c_list[i];
		list = list->next;
		i++;
	}
	
}

void Consumer::search()
{
	cout << "Please input your key words:" << endl;
	string key_words;
	while (key_words.length() == 0)
	{
		getline(cin, key_words);
	}
	commodity_list* head = NULL;
	clist.read_from_txt();
	head = clist.consumer_search_by_key_word(key_words);
	if (head == NULL)
	{
		cout << "\033[31m";
		cout << "There were no relevant commodities." << endl;
		cout << "\033[0m";
		
	}
	else
	{
		cout <<"Sort by relevance to input 1, sort by time to input 2, sort by price to input 3, sort by number to input 4"<< endl;
		int j;
		cin >> j;
		if(j == 1)
		{
			head = head;
		}
		else
		{
			sort(head,j);
		}
		
		cout << left << setw(8) << "Id" << setw(30) << "Name" << setw(8) << "price" << setw(8) << "markup" << setw(8) << "number" << setw(12) << "seller_id" << setw(24) << "releasing time" << setw(12) << "state";
		cout << "\032[0m";
		while (head)
		{
			Time time(head->data.auction_time);
			string str_time = time.time_to_string();
			string state = "OnSale";
			cout << endl << left << setw(8) << head->data.id << setw(30) << head->data.name << setw(8) << head->data.price << setw(8) << head->data.add_price
				<< setw(8) << head->data.num << setw(12) << head->data.user_id << setw(24) << str_time << setw(12) << state;
			head = head->next;
		}
		cout << "\033[0m";
		string judge;
		cout << endl << "Do you want to auction? Input 'yes' to auction." << endl;
		cin >> judge;
		if (judge != "yes")
		{
			
			return;
		}
		else
		{
			cout << endl << "Please input the commodity id you want to auction." << endl;
			string c_id;
			cin >> c_id;
			auction(c_id);
		}
	}
}

void Consumer::cancel_order()
{
	olist.read_from_txt();
	order_list* head = olist.consumer_check(this->get_inform().id);
	order_list* list = head;
	while (list)
	{
		if (list->data.st == Waiting)
		{
			cout << list->data.order_id <<endl;
		}
		list = list->next;
	}
	cout << "Please input the id of the order you want to cancel:" << endl;
	string oid;
	cin >> oid;
	list = head;
	bool flag = false;
	while (list)
	{
		if (list->data.st == Waiting && list->data.order_id == oid)
		{
			flag = true;
			break;
		}
		list = list->next;
	}
	if (!flag)
	{
		cout << "\033[31m";
		cout << "You don't have such an order!" << endl;
		cout << "\033[0m";
		
		return;
	}
	else if(list != NULL)
	{
		cout << "Order id:" << list->data.order_id << endl;
		cout << "Commodity id:" << list->data.commodity_id << endl;
		cout << "Commodity name:" << list->data.commodity_name << endl;
		cout << "Seller id:" << list->data.seller_id << endl;
		Time time(list->data.time);
		cout << "Your bid:" << time.time_to_string() << endl;
		cout << "Number:" << list->data.num << endl;
		cout << "Input 'yes' to affirm." << endl;
		string judge;
		cin >> judge;
		if (judge == "yes")
		{
			olist.cancel_order(oid);
			olist.write_to_txt();
			cout << "You have cancelled the order:" << oid << endl;
		}
		else
		{
			cout << "Operation has been cancelled!" << endl;
		}
		
		return;
	}
}
