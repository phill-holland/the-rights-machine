#include "string.h"

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
		operator string() { return to(); }

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
			const unsigned char a[] = { 31, 28, 31, 30, 31, 30, 31,    31, 30,    31, 30, 31 };
			const unsigned char b[] = { 31, 59, 90, 120, 151, 181, 112, 143, 173, 204, 230, 261 };
			return *this;
		}
	};
};

#endif