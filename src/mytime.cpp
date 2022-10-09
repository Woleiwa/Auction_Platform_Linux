#include "../inc/mytime.h"

Time::Time()
{

}

Time::Time(my_time ori_data)
{
	this->data = ori_data;
}

my_time Time::current_time()
{

	time_t rawtime;
	struct tm* ptminfo;
	time(&rawtime);
	ptminfo = localtime(&rawtime);
	
	this->data.year = ptminfo->tm_year + 1900;
	this->data.month = ptminfo->tm_mon + 1;
	this->data.day = ptminfo->tm_mday;
	this->data.hour = ptminfo->tm_hour;
	this->data.minute = ptminfo->tm_min;
	this->data.second = ptminfo->tm_sec;

	return this->data;
}

bool Time::reach_time_gap()
{
	Time time;
	my_time cur = time.current_time();
	if (cur.year > this->data.year)
	{
		return true;
	}
	else if (cur.month > this->data.month)
	{
		return true;
	}
	else if (cur.day > this->data.day + 1)
	{
		return true;
	}
	else if (cur.day == this->data.day + 1)
	{
		if (cur.hour > this->data.hour)
		{
			return true;
		}
		else if (cur.hour == this->data.hour)
		{
			if (cur.minute > this->data.minute)
			{
				return true;
			}
			else if (cur.minute == this->data.minute)
			{
				if (cur.second > this->data.second)
				{
					return true;
				}
			}
		}
	}
	return false;
}

my_time Time::string_to_time(string stringfied_time)
{
	string year(stringfied_time, 0, 4);
	string month(stringfied_time, 5, 2);
	string day(stringfied_time, 8, 2);
	string hour(stringfied_time, 11, 2);
	string minute(stringfied_time, 14, 2);
	string second(stringfied_time, 17, 2);
	this->data.year = stoi(year);
	this->data.month = stoi(month);
	this->data.day = stoi(day);
	this->data.hour = stoi(hour);
	this->data.minute = stoi(minute);
	this->data.second = stoi(second);
	return this->data;
}

string Time::time_to_string()
{
	string year = to_string(this->data.year);
	string month = to_string(this->data.month);
	string day = to_string(this->data.day);
	string hour = to_string(this->data.hour);
	string minute = to_string(this->data.minute);
	string second = to_string(this->data.second);
	if (month.length() < 2)
	{
		month = '0' + month;
	}
	if (day.length() < 2)
	{
		day = '0' + day;
	}
	if (hour.length() < 2)
	{
		hour = '0' + hour;
	}
	if (minute.length() < 2)
	{
		minute = '0' + minute;
	}
	if (second.length() < 2)
	{
		second = '0' + second;
	}
	string stringfied_time = year + '-' + month + '-' + day + ' ' + hour + ':' + minute + ':' + second;
	return stringfied_time;
}

bool Time::operator < (Time& another)
{
	if(this->data.year < another.data.year)
	{
		return true;
	}
	else if (this->data.year == another.data.year)
	{
		if(this->data.month < another.data.month)
		{
			return true;
		}
		else if(this->data.month == another.data.month)
		{
			if(this->data.day < another.data.day)
			{
				return true;
			}
			else if(this->data.day == another.data.day)
			{
				if(this->data.hour < another.data.hour)
				{
					return true;
				}
				else if(this->data.hour == another.data.hour)
				{
					if(this->data.minute < another.data.minute)
					{
						return true;
					}
					else if(this->data.minute == another.data.minute)
					{
						if(this->data.second < another.data.second)
						{
							return true;
						}
					}
				}
			}
		}
	}
	return false;
}
