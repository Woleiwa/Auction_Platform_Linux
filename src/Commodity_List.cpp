#include"../inc/Commodity_List.h"
#include"../inc/commodity.h"
#include"../inc/order.h"
#include<cstring>
#include<iomanip>
#include<iostream>
#include<fstream>
#include<mutex>
#include<cstdlib>
#include"../inc/Order_List.h"
extern mutex c_mtx;

Commodity_List::Commodity_List()
{

}

Commodity_List::~Commodity_List()
{
	while (head)
	{
		commodity_list* cur = head;
		head = head->next;
		delete cur;
	}
}

static commodity_inform string_to_info(string str_info)
{
	int head = 0;
	int rear = 0;
	rear = str_info.find(',', head);
	string id(str_info, head, rear - head);

	head = rear + 1;
	rear = str_info.find(',', head);
	string name(str_info, head, rear - head);

	head = rear + 1;
	rear = str_info.find(',', head);
	string price(str_info, head, rear - head);

	head = rear + 1;
	rear = str_info.find(',', head);
	string add_pirce(str_info, head, rear - head);

	head = rear + 1;
	rear = str_info.find(',', head);
	string str_num(str_info, head, rear - head);

	head = rear + 1;
	rear = str_info.find(',', head);
	string str_time(str_info, head, rear - head);

	head = rear + 1;
	rear = str_info.find(',', head);
	string user_id(str_info, head, rear - head);

	head = rear + 1;
	rear = str_info.find(',', head);
	string st(str_info, head, rear - head);

	head = rear + 1;
	rear = str_info.length();
	string description(str_info, head, rear - head);

	commodity_inform res;
	strcpy(res.id, id.c_str());
	strcpy(res.name, name.c_str());

	res.price = stod(price);
	res.add_price = stod(add_pirce);
	res.num = stoi(str_num);

	Time time;
	res.auction_time = time.string_to_time(str_time);

	strcpy(res.user_id, user_id.c_str());
	strcpy(res.description, description.c_str());

	res.st = OnSale;
	if (st == "Sold")
	{
		res.st = Sold;
	}
	else if (st == "OffShelf")
	{
		res.st = OffShelf;
	}
	else if (st == "Timeout")
	{
		res.st = Timeout;
	}
	return res;
}

static string info_to_string(commodity_inform info)
{
	string id = info.id;
	string name = info.name;
	string price = to_string(info.price);
	string add_price = to_string(info.add_price);
	string str_num = to_string(info.num);
	Time time(info.auction_time);
	string str_time = time.time_to_string();
	string user_id = info.user_id;
	string description = info.description;
	string st = "OnSale";
	if (info.st == Sold)
	{
		st = "Sold";
	}
	else if (info.st == OffShelf)
	{
		st = "OffShelf";
	}
	else if (info.st == Timeout)
	{
		st = "Timeout";
	}
	string str_info = id + ',' + name + ',' + price + ',' + add_price + ',' + str_num + ',' + str_time + ',' + user_id + ',' + st + ',' + description;
	return str_info;
}

bool Commodity_List::read_from_txt()
{
	c_mtx.lock();
	commodity_list* cur = this->head;
	while (cur)
	{
		commodity_list* temp = cur;
		cur = cur->next;
		delete temp;
	}
	this->head = NULL;
	this->tail = NULL;
	this->num = 0;
	char buffer[512];
	ifstream commofile("commodity.txt");
	if (!commofile.is_open())
	{
		c_mtx.unlock();
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
		commodity_inform commo_info = string_to_info(info);
		if (head == NULL)
		{
			head = new commodity_list;
			head->data = commo_info;
			head->next = NULL;
			tail = head;
		}
		else
		{
			tail->next = new commodity_list;
			tail = tail->next;
			tail->data = commo_info;
			tail->next = NULL;
		}
	}

	commodity_list* head = this->head;
	while (head)
	{
		num++;
		head = head->next;
	}
	c_mtx.unlock();
	return true;
}

void Commodity_List::write_to_txt()
{
	c_mtx.lock();
	ofstream userfile("commodity.txt");
	if (!userfile.is_open())
	{
		c_mtx.unlock();
		return;
	}
	else
	{
		commodity_list* commo_head = head;
		while (commo_head != NULL)
		{
			string info_str = info_to_string(commo_head->data);
			userfile << info_str + '\n';
			commo_head = commo_head->next;
		}
	}
	c_mtx.unlock();
}

void Commodity_List::add_commodity(commodity_inform& info)
{
	if (tail == NULL)
	{
		string id = "M00001";
		strcpy(info.id, id.c_str());
		tail = new commodity_list;
		head = tail;
		head->next = NULL;
		head->data = info;
	}
	else
	{
		string last_id(tail->data.id, 1, 5);
		int num = stoi(last_id) + 1;
		string inum = to_string(num);
		while (inum.length() < 5)
		{
			inum = '0' + inum;
		}
		string id = "M" + inum;
		strcpy(info.id, id.c_str());
		tail->next = new commodity_list;
		tail = tail->next;
		tail->next = NULL;
		tail->data = info;
	}
	write_to_txt();
}

commodity_list* Commodity_List::consumer_check(char id[20])
{
	commodity_list* head = this->head;
	commodity_list* reshead = NULL;
	commodity_list* restail = NULL;
	string uid = id;
	while (head)
	{
		if (head->data.st == OnSale && head->data.user_id != uid)
		{
			if (reshead == NULL)
			{
				reshead = new commodity_list;
				restail = reshead;
			}
			else
			{
				restail->next = new commodity_list;
				restail = restail->next;
			}
			restail->data = head->data;
			restail->next = NULL;
		}
		head = head->next;
	}
	return reshead;
}

commodity_list* Commodity_List::seller_check(char id[20])
{
	commodity_list* head = this->head;
	commodity_list* reshead = NULL;
	commodity_list* restail = NULL;
	string uid = id;
	while (head)
	{
		if (head->data.user_id == uid)
		{
			if (reshead == NULL)
			{
				reshead = new commodity_list;
				restail = reshead;
			}
			else
			{
				restail->next = new commodity_list;
				restail = restail->next;
			}
			restail->data = head->data;
			restail->next = NULL;
		}
		head = head->next;
	}
	return reshead;
}

commodity_list* Commodity_List::admin_check()
{
	return this->head;
}

commodity_inform Commodity_List::admin_search_by_id(string id)
{
	commodity_list* head = this->head;
	commodity_inform res;
	while (head)
	{
		if (head->data.id == id)
		{
			res = head->data;
			break;
		}
		head = head->next;
	}
	return res;
}

commodity_inform Commodity_List::consumer_search_by_id(string id)
{
	commodity_list* head = this->head;
	commodity_inform res;
	while (head)
	{
		if (head->data.id == id && head->data.st == OnSale)
		{
			res = head->data;
			break;
		}
		head = head->next;
	}
	return res;
}

commodity_list* Commodity_List::find_one_commodity(string cid)
{
	commodity_list* head = this->head;
	while (head)
	{
		if (head->data.id == cid)
		{
			return head;
		}
		head = head->next;
	}
	return head;
}

void Commodity_List::freeze_user(string uid)
{
	commodity_list* head = this->head;
	while (head)
	{
		if (head->data.user_id == uid && head->data.st == OnSale)
		{
			head->data.st = OffShelf;
		}
		head = head->next;
	}
}

bool KMP_vague_match(string key, string target)
{
	int j = 0, k = -1;
	int l = key.length();
	int* next = new int[l + 1];
	next[0] = -1;
	while (j < l)
	{
		if (k == -1 || key[j] == key[k] || key[j] == key[k] + 32 || key[j] == key[k] - 32)
		{
			j++;
			k++;
			next[j] = k;
		}
		else
		{
			k = next[k];
		}
	}
	int l2 = target.length();
	int i = 0;
	j = 0;
	while (i < l2 && j < l)
	{
		if (j == -1 || target[i] == key[j] || target[i] == key[j] + 32 || target[i] == key[j] - 32)
		{
			j++;
			i++;
		}
		else
		{
			j = next[j];
		}
	}
	if (j >= l)
	{
		return true;
	}
	return false;
}

void Merge_sort(commodity_relevant list[], int begin, int end)
{
	if (begin >= end)
	{
		return;
	}
	int mid = (begin + end) / 2;
	Merge_sort(list, begin, mid);
	Merge_sort(list, mid + 1, end);
	int i = begin;
	int j = mid + 1;
	int cur = 0;
	commodity_relevant* order_list = new commodity_relevant[end - begin + 1];
	while (i <= mid && j <= end)
	{
		if (list[i].relevance >= list[j].relevance)
		{
			order_list[cur++] = list[i++];
		}
		else
		{
			order_list[cur++] = list[j++];
		}
	}
	if (i <= mid)
	{
		while (i <= mid)
		{
			order_list[cur++] = list[i++];
		}
	}
	else
	{
		while (j <= end)
		{
			order_list[cur++] = list[j++];
		}
	}
	for (int i = 0; i < end - begin + 1; i++)
	{
		list[begin + i] = order_list[i];
	}
}

void Commodity_List::set_relevance(commodity_relevant list[], string keys)
{
	int cur = 0;
	int key_num = 1;
	while (keys.find(' ', cur) != -1)
	{
		cur = keys.find(' ', cur) + 1;
		key_num++;
	}
	string* key = new string[key_num];

	cur = 0;
	for (int i = 0; i < key_num - 1; i++)
	{
		int end = keys.find(' ', cur);
		string cur_key(keys.c_str(), cur, end - cur);
		key[i] = cur_key;
		cur = end + 1;
	}

	int end = keys.length();
	string cur_key(keys.c_str(), cur, end - cur);
	key[key_num - 1] = cur_key;

	commodity_list* head = this->head;

	for (int i = 0; i < this->num; i++)
	{
		list[i].data = head->data;
		head = head->next;
		list[i].relevance = 0;
		int match_num = 0;
		for (int j = 0; j < key_num; j++)
		{
			if (KMP_vague_match(key[j], list[i].data.name))
			{
				match_num++;
			}
		}
		list[i].relevance = (double)match_num / (double)key_num;
	}
	Merge_sort(list, 0, this->num - 1);
}

commodity_list* Commodity_List::admin_search_by_key_word(string key)
{
	commodity_relevant* list = new commodity_relevant[this->num];
	set_relevance(list, key);
	commodity_list* res_head = NULL, * res_tail = NULL;
	int i = 0;
	while (i < this->num)
	{
		if (list[i].relevance > 0)
		{
			if (res_head == NULL)
			{
				res_head = new commodity_list;
				res_tail = res_head;
			}
			else
			{
				res_tail->next = new commodity_list;
				res_tail = res_tail->next;
			}
			res_tail->data = list[i].data;
			res_tail->next = NULL;
			i++;
		}
		else
		{
			break;
		}
	}
	return res_head;
}

commodity_list* Commodity_List::consumer_search_by_key_word(string key)
{
	commodity_relevant* list = new commodity_relevant[this->num];
	set_relevance(list, key);
	commodity_list* res_head = NULL, * res_tail = NULL;
	int i = 0;
	while (i < this->num)
	{
		if (list[i].relevance > 0)
		{
			if (list[i].data.st == OnSale)
			{
				if (res_head == NULL)
				{
					res_head = new commodity_list;
					res_tail = res_head;
				}
				else
				{
					res_tail->next = new commodity_list;
					res_tail = res_tail->next;
				}
				res_tail->data = list[i].data;
				res_tail->next = NULL;
			}
			i++;
		}
		else
		{
			break;
		}
	}
	return res_head;
}

bool Commodity_List::update()
{
	commodity_list* head = this->head;
	bool res = false;
	while (head)
	{
		if (head->data.st == OnSale)
		{
			Time time(head->data.auction_time);
			if (time.reach_time_gap())
			{
				Order_List upolist;
				upolist.read_from_txt();
				int num = upolist.update(head->data);
				if ((head->data.num -= num) == 0)
				{
					head->data.st = Sold;
				}
				else
				{
					head->data.st = Timeout;
				}
				upolist.write_to_txt();
				res = true;
			}
		}
		head = head->next;
	}
	return res;
}
