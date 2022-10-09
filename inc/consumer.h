#pragma once
#include "user.h"
#ifndef _CONSUMER_
#define _CONSUMER_
class Consumer: public User
{
public:
	using User::User;

	void operate();

	void check_order();

	void check_commodity();

	void auction();

	void get_information();

	void search();

	void auction(string id);

	void cancel_order();
};

#endif