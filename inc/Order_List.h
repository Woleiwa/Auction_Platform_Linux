#pragma once
#include"order.h"
#include"commodity.h"
#ifndef _ORDERLIST_
#define _ORDERLIST_

class Order_List
{
private:
	order_list* head = NULL, * tail = NULL;
public:
	bool read_from_txt();

	void write_to_txt();

	void add_to_list(order& info);

	order_list* admin_check();

	order_list* consumer_check(string uid);

	order_list* seller_check(string uid);

	//contacter* my_contacter(string uid);

	double max_price(string commodity_id);

	void offshelf(string cid);

	void freeze_user(string id);

	int update(commodity_inform info);

	void modify_information(string cid, string new_name);

	void cancel_order(string oid);
};
#endif // !_ORDERLIST_



