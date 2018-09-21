#include "string.h"
#include <Windows.h>
#include <sqltypes.h>

#if !defined(__DATETIME)
#define __DATETIME

namespace global
{
	class datetime
	{
		int year, month, day;
		int hour, minute, second;

	public:
		datetime() { clear(); }
		datetime(string const &source) { clear(); from((string)source); }
		datetime(datetime const &source) { clear(); copy(source); }
		datetime(time_t const &source)
		{
			tm l;
			localtime_s(&l, &source);

			year = l.tm_year + 1900;
			month = l.tm_mon + 1;
			day = l.tm_mday;

			hour = l.tm_hour;
			minute = l.tm_min;
			second = l.tm_sec;
		}
		datetime(TIMESTAMP_STRUCT const &source)
		{
			year = (int)source.year;
			month = (int)source.month;
			day = (int)source.day;

			hour = (int)source.hour;
			minute = (int)source.minute;
			second = (int)second;
		}

		datetime(int y, int m, int d, int hh = 0, int mm = 0, int ss = 0)
		{
			clear();

			year = y;
			month = m;
			day = d;

			hour = hh;
			minute = mm;
			second = ss;
		}

		void clear();

		static datetime now();

		bool from(string &source);
		string to();

		void copy(datetime const &source);

	public:
		operator time_t()
		{
			tm temp = (tm)*this;

			return mktime(&temp);
		}

		operator tm()
		{
			struct tm l = { 0 };

			l.tm_year = year > 0 ? (year - 1900) : 0;
			l.tm_mon = month - 1;
			l.tm_mday = day;

			l.tm_hour = hour;
			l.tm_min = minute;
			l.tm_sec = second;

			return l;
		}

		operator TIMESTAMP_STRUCT()
		{
			TIMESTAMP_STRUCT ts = { 0 };

			ts.year = (SQLSMALLINT)year;
			ts.month = (SQLSMALLINT)month;
			ts.day = (SQLSMALLINT)day;

			ts.hour = (SQLUSMALLINT)hour;
			ts.minute = (SQLUSMALLINT)minute;
			ts.day = (SQLUSMALLINT)day;

			ts.second = (SQLUSMALLINT)0;
			ts.fraction = (SQLUINTEGER)0;

			return ts;
		}

		operator string() { return to(); }

	public:
		datetime& operator=(const datetime& source)
		{
			this->copy((datetime&)source);
			return *this;
		}

		bool operator==(const datetime& source)
		{ 
			return day == source.day &&
				   month == source.month &&
				   year == source.year &&
				   hour == source.hour &&
				   minute == source.minute &&
				   second == source.second;
		}

		bool operator!=(const datetime& source)
		{ 
			return day != source.day ||
				   month != source.month ||
				   year != source.year || 
				   hour != source.hour ||
				   minute != source.minute ||
				   second != source.second;
		}

		bool operator<(const datetime& source)
		{ 			
			if (year < source.year) return true;
			if ((year == source.year) && (month < source.month)) return true;
			if ((month == source.month) && (day < source.day)) return true;
			if ((day == source.day) && (hour < source.hour)) return true;
			if ((hour == source.hour) && (minute < source.minute)) return true;
			if ((minute == source.minute) && (second < source.second)) return true;

			return false;
		}

		bool operator>(const datetime& source)
		{ 
			if (year > source.year) return true;
			if ((year == source.year) && (month > source.month)) return true;
			if ((month == source.month) && (day > source.day)) return true;
			if ((day == source.day) && (hour > source.hour)) return true;
			if ((hour == source.hour) && (minute > source.minute)) return true;
			if ((minute == source.minute) && (second > source.second)) return true;

			return false;
		}

		bool operator<=(const datetime& source)
		{ 
			if (year < source.year) return true;
			if ((year == source.year) && (month < source.month)) return true;
			if ((month == source.month) && (day <= source.day)) return true;
			if ((day == source.day) && (hour <= source.hour)) return true;
			if ((hour == source.hour) && (minute <= source.minute)) return true;
			if ((minute == source.minute) && (second <= source.second)) return true;

			return false;
		}

		bool operator>=(const datetime& source)
		{ 
			if (year > source.year) return true;
			if ((year == source.year) && (month > source.month)) return true;
			if ((month == source.month) && (day >= source.day)) return true;
			if ((day == source.day) && (hour >= source.hour)) return true;
			if ((hour == source.hour) && (minute >= source.minute)) return true;
			if ((minute == source.minute) && (second >= source.second)) return true;

			return false;
		}

		datetime& operator+(const datetime& source)
		{
			tm a = (tm)*this;
			tm b = (tm)(datetime)source;

			a.tm_year += b.tm_year;
			a.tm_mon += b.tm_mon;
			a.tm_mday += b.tm_mday;
			a.tm_hour += b.tm_hour;
			a.tm_min += b.tm_min;
			a.tm_sec += b.tm_sec;

			time_t c = mktime(&a);
			this->copy((datetime)c);
			
			return *this;
		}

		datetime& operator-(const datetime& source)
		{
			tm a = (tm)*this;
			tm b = (tm)(datetime)source;

			a.tm_year -= b.tm_year;
			a.tm_mon -= b.tm_mon;
			a.tm_mday -= b.tm_mday;
			a.tm_hour -= b.tm_hour;
			a.tm_min -= b.tm_min;
			a.tm_sec -= b.tm_sec;

			time_t c = mktime(&a);
			this->copy((datetime)c);

			return *this;
		}
	};
};

#endif