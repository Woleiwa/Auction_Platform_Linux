#include"../inc/Order_List.h"
#include<mutex>
#include"../inc/commodity.h"
#include"../inc/User_List.h"
#include<vector>
#include<algorithm>

extern mutex o_mtx;

static order string_to_info(string str_info)
{
	int head = 0;
	int rear = 0;
	rear = str_info.find(',', head);
	string order_id(str_info, head, rear - head);

	head = rear + 1;
	rear = str_info.find(',', head);
	string commodity_id(str_info, head, rear - head);

	head = rear + 1;
	rear = str_info.find(',', head);
	string commodity_name(str_info, head, rear - head);

	head = rear + 1;
	rear = str_info.find(',', head);
	string seller_id(str_info, head, rear - head);

	head = rear + 1;
	rear = str_info.find(',', head);
	string auctioneer_id(str_info, head, rear - head);

	head = rear + 1;
	rear = str_info.find(',', head);
	string str_time(str_info, head, rear - head);

	head = rear + 1;
	rear = str_info.find(',', head);
	string bid(str_info, head, rear - head);

	head = rear + 1;
	rear = str_info.find(',', head);
	string str_num(str_info, head, rear - head);

	head = rear + 1;
	rear = str_info.length();
	string str_st(str_info, head, rear - head);

	order res;
	strcpy(res.order_id, order_id.c_str());
	strcpy(res.commodity_id, commodity_id.c_str());
	strcpy(res.auctioneer, auctioneer_id.c_str());
	strcpy(res.commodity_name, commodity_name.c_str());
	strcpy(res.seller_id, seller_id.c_str());
	res.bid = stod(bid);
	res.num = stoi(str_num);
	res.st = Waiting;
	if (str_st == "Deal")
	{
		res.st = Deal;
	}
	else if (str_st == "Cancel")
	{
		res.st = Cancel;
	}
	Time time;
	res.time = time.string_to_time(str_time);
	return res;
}

static string info_to_string(order info)
{
	string order_id = info.order_id;
	string c_id = info.commodity_id;
	string c_name = info.commodity_name;
	string s_id = info.seller_id;
	string a_id = info.auctioneer;
	Time time(info.time);
	string str_time = time.time_to_string();
	string bid = to_string(info.bid);
	string str_num = to_string(info.num);
	string state = "Waiting";
	if (info.st == Deal)
	{
		state = "Deal";
	}
	else if (info.st == Cancel)
	{
		state = "Cancel";
	}
	string str_info = order_id + ',' + c_id + ',' + c_name + ',' + s_id + ',' + a_id + ',' + str_time + ',' + bid + ',' + str_num + ',' + state;
	return str_info;
}

bool Order_List::read_from_txt()
{
	o_mtx.lock();
	this->head = NULL;
	this->tail = NULL;
	order_list* cur = this->head;
	while (cur)
	{
		order_list* temp = cur;
		cur = cur->next;
		delete temp;
	}
	char buffer[512];
	ifstream commofile("order.txt");
	if (!commofile.is_open())
	{
		o_mtx.unlock();
		return false;
	}

	while (!commofile.eof())
	{
		commofile.getline(buffer, 511);
		if (strlen(buffer) <= 1)
		{
			break;
		}

		string info(buffer);
		order order_info = string_to_info(info);
		if (head == NULL)
		{
			head = new order_list;
			head->data = order_info;
			head->next = NULL;
			tail = head;
		}
		else
		{
			tail->next = new order_list;
			tail = tail->next;
			tail->data = order_info;
			tail->next = NULL;
		}
	}
	o_mtx.unlock();
	return true;

}

void Order_List::write_to_txt()
{
	o_mtx.lock();
	ofstream userfile("order.txt");
	if (!userfile.is_open())
	{
		o_mtx.unlock();
		return;
	}
	else
	{
		order_list* order_head = head;
		while (order_head != NULL)
		{
			string info_str = info_to_string(order_head->data);
			userfile << info_str + '\n';
			order_head = order_head->next;
		}
	}
	o_mtx.unlock();
}

void Order_List::add_to_list(order& info)
{
	if (this->head == NULL)
	{
		string order_id = "O00001";
		strcpy(info.order_id, order_id.c_str());
		this->head = new order_list;
		this->tail = this->head;
	}
	else
	{
		string order_id(this->tail->data.order_id, 1, 5);
		int num = stoi(order_id) + 1;
		order_id = to_string(num);
		while (order_id.length() < 5)
		{
			order_id = '0' + order_id;
		}
		order_id = 'O' + order_id;
		strcpy(info.order_id, order_id.c_str());
		this->tail->next = new order_list;
		this->tail = this->tail->next;
	}
	this->tail->data = info;
	this->tail->next = NULL;
}

order_list* Order_List::admin_check()
{
	return this->head;
}

order_list* Order_List::consumer_check(string uid)
{
	order_list* check_head = this->head;
	order_list* res_head = NULL;
	order_list* res_tail = NULL;
	while (check_head)
	{
		if (check_head->data.auctioneer == uid)
		{
			if (res_head == NULL)
			{
				res_head = new order_list;
				res_tail = res_head;
			}
			else
			{
				res_tail->next = new order_list;
				res_tail = res_tail->next;
			}
			res_tail->data = check_head->data;
			res_tail->next = NULL;
		}
		check_head = check_head->next;
	}
	return res_head;
}

order_list* Order_List::seller_check(string uid)
{
	order_list* check_head = this->head;
	order_list* res_head = NULL;
	order_list* res_tail = NULL;
	while (check_head)
	{
		if (check_head->data.seller_id == uid)
		{
			if (res_head == NULL)
			{
				res_head = new order_list;
				res_tail = res_head;
			}
			else
			{
				res_tail->next = new order_list;
				res_tail = res_tail->next;
			}
			res_tail->data = check_head->data;
			res_tail->next = NULL;
		}
		check_head = check_head->next;

	}
	return res_head;
}

double Order_List::max_price(string commodity_id)
{
	order_list* check_head = this->head;
	Commodity commodity(commodity_id);
	double price = commodity.my_info().price;
	while (check_head)
	{
		if (check_head->data.commodity_id == commodity_id && check_head->data.st == Waiting)
		{
			if (check_head->data.bid > price)
			{
				price = check_head->data.bid;
			}
		}
		check_head = check_head->next;
	}
	return price;
}

void Order_List::offshelf(string cid)
{
	order_list* head = this->head;
	while (head)
	{
		if (head->data.commodity_id == cid && head->data.st == Waiting)
		{
			head->data.st = Cancel;
		}
		head = head->next;
	}
}

void Order_List::freeze_user(string id)
{
	order_list* head = this->head;
	while (head)
	{
		if ((head->data.auctioneer == id || head->data.seller_id == id) && head->data.st == Waiting)
		{
			head->data.st = Cancel;
		}
		head = head->next;
	}
}

int Order_List::update(commodity_inform info)
{
	int res = 0;
	order_list* head = this->head;
	vector<order>olist;
	vector<order_list*>reslist;
	string c_id = info.id;
	while (head)
	{
		if (head->data.commodity_id == c_id && head->data.st == Waiting)
		{
			olist.push_back(head->data);
		}
		reslist.push_back(head);
		head = head->next;
	}
	if (olist.empty())
	{
		return res;
	}
	sort(olist.begin(), olist.end());
	reverse(olist.begin(), olist.end());
	User_List uplist;
	uplist.read_from_txt();
	for (int i = 0; i < olist.size() && info.num > 0; i++)
	{
		int num = 0;
		if (olist[i].num < info.num)
		{
			num = olist[i].num;
			info.num -= num;
		}
		else
		{
			num = info.num;
			info.num = 0;
		}
		res += num;
		double cost = num * olist[i].bid;
		string str_id(olist[i].order_id, 1, 5);
		int id = stoi(str_id);
		reslist[id - 1]->data.st = Deal;
		reslist[id - 1]->data.num = num;
		uplist.update(olist[i].seller_id, olist[i].auctioneer, cost);
	}
	uplist.write_to_txt();
	return res;
}

void Order_List::modify_information(string cid, string new_name)
{
	order_list* list = this->head;
	while (list)
	{
		if (list->data.commodity_id == cid)
		{
			strcpy(list->data.commodity_name, new_name.c_str());
		}
		list = list->next;
	}
}

void Order_List::cancel_order(string oid)
{
	order_list* list = this->head;
	while (list)
	{
		if (list->data.order_id == oid)
		{
			list->data.st = Cancel;
			break;
		}
		list = list->next;
	}
}
