#pragma once
#ifndef _MY_TIME_
#define _MY_TIME_
#include <cstring>
#include <iostream>
#include <time.h>
#include <string>
#include <cstdlib>
using namespace std;

struct my_time
{
	int year = 1970;
	int month = 0;
	int day = 0;
	int hour = 0;
	int minute = 0;
	int second = 0;
};

class Time
{
private:
	my_time data;
public:
	Time();

	Time(my_time data);

	my_time current_time();

	bool reach_time_gap();

	my_time string_to_time(string stringfied_time);

	string time_to_string();

	bool operator < (Time& another);
};
#endif // !_MY_TIME_



