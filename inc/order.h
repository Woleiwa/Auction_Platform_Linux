#pragma once
#ifndef _ORDER_
#define _ORDER_
#include<cstring>
#include<iomanip>
#include<iostream>
#include<fstream>
#include<string>
#include<cstdlib>

#include"mytime.h"
enum order_state{Deal, Cancel, Waiting};
struct order
{
	char order_id[20];
	char seller_id[20];
	char auctioneer[20];
	char commodity_id[20];
	char commodity_name[50];
	int num = 1;
	my_time time;
	double bid;
	order_state st;

	bool operator>(order other)
	{
		return this->bid > other.bid;
	}

	bool operator<(order other)
	{
		return this->bid < other.bid;
	}
};

struct order_list
{
	order data;
	order_list* next;
};

class Order
{
private:
	order info;
public:
	Order(string seller_id, string auctioneer_id, string commodity_id, string commodity_name, double bid, int num);
	
	Order(order info);

	order my_info(order info);

	void add_to_olist();
};

#endif // !_ORDER_



