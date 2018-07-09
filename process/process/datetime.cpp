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

bool global::datetime::from(string &source)
{
	char yy[] = { '0', '0', '0', '0', 0 };
	char mm[] = { '0', '0', 0 };
	char dd[] = { '0', '0', 0 };

	char *dest[] = { yy, mm, dd };
	long length[] = { 3, 1, 1 };

	long ptr = 2L;
	long idx = 1L;

	for (long i = source.count() - 1L; i >=0L; i--)
	{
		if (ptr < 0L) return false;
		if (source[i] != '-')
		{
			if (!((source[i] >= '0') && (source[i] <= '9'))) return false;
			dest[ptr][idx--] = source[i];
		}
		else
		{
			--ptr;
			if (ptr > 0L) idx = length[ptr];
		}

		if (idx < 0L)
		{
			--ptr;
			if (ptr > 0) idx = length[ptr];
		}
	}

	year = string(yy).toInteger();
	month = string(mm).toInteger();
	day = string(dd).toInteger();

	return true;
}

string global::datetime::to()
{
	return string("");
}

global::datetime global::datetime::now()
{
	datetime result;

	std::time_t tt = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
	tm l;
	localtime_s(&l, &tt);

	result.year = l.tm_year;
	result.month = l.tm_mon;
	result.day = l.tm_mday;

	result.hour = l.tm_hour;
	result.minute = l.tm_min;
	result.second = l.tm_sec;

	return result;
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

