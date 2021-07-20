#include "datetime.h"
#include <chrono>
#include <ctime>

void global::datetime::clear()
{
	year = 0;
	month = 0;
	day = 0;
	hour = 0;
	minute = 0;
	second = 0;
}

bool global::datetime::from(string const &source)
{
	char yy[] = { '0', '0', '0', '0', 0 };
	char mm[] = { '0', '0', 0 };
	char dd[] = { '0', '0', 0 };

	char *dest[] = { yy, mm, dd };
	long length[] = { 3, 1, 1 };

	long ptr = 2L;
	long idx = 1L;

	for (long i = (long)source.length() - 1L; i >=0L; i--)
	{
		if (ptr < 0L) return false;

		if (source[i] == '-')
		{
			--ptr;
			if (ptr >= 0L) idx = length[ptr];
		}
		
		if (idx < 0L)
		{
			--ptr;
			if (ptr >= 0L) idx = length[ptr];
		}

		if (source[i] != '-')
		{
			if (!((source[i] >= '0') && (source[i] <= '9'))) return false;
			dest[ptr][idx--] = source[i];
		}
	}

	year = string(yy).toInteger();
	month = string(mm).toInteger();
	day = string(dd).toInteger();

	return true;
}

string global::datetime::to()
{
	string result = string::fromInt(year);
	result += "-";
	result += padding(string::fromInt(month), 2);
	result += "-";
	result += padding(string::fromInt(day), 2);
	result += "T";
	result += padding(string::fromInt(hour), 2);
	result += ":";
	result += padding(string::fromInt(minute), 2);
	result += ":";
	result += padding(string::fromInt(second), 2);

	return result;
}

string global::datetime::padding(string source, long count)
{
	if(source.size() >= count) return source;
	string result;

	for(long i = 0L; i < count - source.size(); ++i)
	{
		result.concat(string("0"));
	}

	result.concat(source);
	return result;
}

global::datetime global::datetime::now()
{
	std::time_t tt = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
	
	return datetime(tt);
}

void global::datetime::copy(datetime const &source)
{
	year = source.year;
	month = source.month;
	day = source.day;

	hour = source.hour;
	minute = source.minute;
	second = source.second;
}

