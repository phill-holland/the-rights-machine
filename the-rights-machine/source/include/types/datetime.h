#include "core/string/string.h"
#include <sqltypes.h>

#if !defined(_TYPES_DATETIME)
#define _TYPES_DATETIME

namespace types
{
	class datetime
	{
		int year, month, day;
		int hour, minute, second;

	public:
		datetime() { clear(); }
		datetime(string const &source) { clear(); from(source); }
		datetime(datetime const &source) { clear(); copy(source); }
		datetime(time_t const &source)
		{
			tm l;
			localtime_r(&source, &l);

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

		bool from(string const &source);
		string to();

		void copy(datetime const &source);

	protected:
		string padding(string source, long count);
		
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
			tm a = (tm)*this;
			tm b = (tm)(datetime)source;

			time_t as = mktime(&a); 		
			time_t bs = mktime(&b);

			return as < bs;
		}

		bool operator>(const datetime& source)
		{ 
			tm a = (tm)*this;
			tm b = (tm)(datetime)source;

			time_t as = mktime(&a); 		
			time_t bs = mktime(&b);

			return as > bs;
		}

		bool operator<=(const datetime& source)
		{ 
			tm a = (tm)*this;
			tm b = (tm)(datetime)source;

			time_t as = mktime(&a); 		
			time_t bs = mktime(&b);

			return as <= bs;
		}

		bool operator>=(const datetime& source)
		{ 
			tm a = (tm)*this;
			tm b = (tm)(datetime)source;

			time_t as = mktime(&a); 		
			time_t bs = mktime(&b);

			return as >= bs;
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

		datetime& operator+(const int& days)
		{
			const time_t ONE_DAY = 24 * 60 * 60;

			tm a = (tm)*this;
			time_t date_seconds = mktime(&a) + (days * ONE_DAY);
			this->copy((datetime)date_seconds);

			return *this;
		}
	};
};

#endif