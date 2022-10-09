#pragma once
#include"commodity.h"
#ifndef _COMMODITYLIST_
#define _COMMODITYLIST_

class Commodity_List
{
public:
	Commodity_List();

	~Commodity_List();

	bool read_from_txt();

	void write_to_txt();

	void add_commodity(commodity_inform& info);

	commodity_inform admin_search_by_id(string id);

	commodity_inform consumer_search_by_id(string id);

	void set_relevance(commodity_relevant list[], string keys);

	commodity_list* admin_search_by_key_word(string key);

	commodity_list* consumer_search_by_key_word(string key);

	commodity_list* seller_check(char id[20]);

	commodity_list* admin_check();

	commodity_list* consumer_check(char id[20]);

	commodity_list* find_one_commodity(string cid);

	void freeze_user(string uid);

	bool update();
private:
	commodity_list* head = NULL, * tail = NULL;
	int num = 0;
};

#endif // !_CommodityList_



