#include"../inc/commodity.h"
#include"../inc/order.h"
#include<cstring>
#include<iomanip>
#include<iostream>
#include<fstream>
#include<mutex>
#include<cstdlib>
#include"../inc/Commodity_List.h"
#include"../inc/Order_List.h"
extern Order_List olist;
extern Commodity_List clist;
extern mutex c_mtx;

Commodity::Commodity(string name, double price, double add_pirce, int num,string description, string uid)
{
	strcpy(this->info.name, name.c_str());
	strcpy(this->info.description, description.c_str());
	strcpy(this->info.user_id, uid.c_str());
	this->info.price = price;
	this->info.num = num;
	this->info.add_price = add_pirce;
}

Commodity::Commodity(string id)
{
	Commodity_List c_list;
	c_list.read_from_txt();
	commodity_inform info = c_list.admin_search_by_id(id);
	this->info = info;
}

commodity_inform Commodity::my_info()
{
	return this->info;
}

void Commodity::create_commodity()
{
	Time mytime;
	this->info.auction_time = mytime.current_time();
	this->info.st = OnSale;
	clist.read_from_txt();
	clist.add_commodity(this->info);
}

void Commodity::offshelf()
{
	this->info.st = OffShelf;
	clist.read_from_txt();
	string cid = this->info.id;
	commodity_list* pointer = clist.find_one_commodity(cid);
	pointer->data.st = OffShelf;
	clist.write_to_txt();
	olist.read_from_txt();
	olist.offshelf(cid);
	olist.write_to_txt();
}

void Commodity::modify_information(string judge, string new_info)
{
	clist.read_from_txt();
	string cid = this->info.id;
	commodity_list* pointer = clist.find_one_commodity(cid);
	if (judge == "1")
	{
		strcpy(this->info.name, new_info.c_str());
		strcpy(pointer->data.name, new_info.c_str());
		olist.read_from_txt();
		olist.modify_information(cid, new_info);
		olist.write_to_txt();
	}
	else
	{
		strcpy(this->info.description, new_info.c_str());
		strcpy(pointer->data.description, new_info.c_str());
	}
	clist.write_to_txt();
}

void Commodity::commodity_re_onshelf()
{
	this->info.st = OnSale;
	Time cur;
	this->info.auction_time = cur.current_time();
	clist.read_from_txt();
	string cid = this->info.id;
	commodity_list* pointer = clist.find_one_commodity(cid);
	pointer->data.st = OnSale;
	pointer->data.auction_time = this->info.auction_time;
	clist.write_to_txt();
}
