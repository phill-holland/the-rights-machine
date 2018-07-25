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
		datetime(int y, int m, int d)
		{
			clear();

			year = y;
			month = m;
			day = d;
		}

		void clear();

		datetime now();

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
				   year == source.year;
		}

		bool operator!=(const datetime& source)
		{ 
			return day != source.day ||
				   month != source.month ||
				   year != source.year;
		}

		bool operator<(const datetime& source)
		{ 			
			if (year < source.year) return true;
			if ((year == source.year) && (month < source.month)) return true;
			if ((month == source.month) && (day < source.day)) return true;

			return false;
		}

		bool operator>(const datetime& source)
		{ 
			if (year > source.year) return true;
			if ((year == source.year) && (month > source.month)) return true;
			if ((month == source.month) && (day > source.day)) return true;

			return false;
		}

		bool operator<=(const datetime& source)
		{ 
			if (year < source.year) return true;
			if ((year == source.year) && (month < source.month)) return true;
			if ((month == source.month) && (day <= source.day)) return true;

			return false;
		}

		bool operator>=(const datetime& source)
		{ 
			if (year > source.year) return true;
			if ((year == source.year) && (month > source.month)) return true;
			if ((month == source.month) && (day >= source.day)) return true;

			return false;
		}
	};
};

#endif