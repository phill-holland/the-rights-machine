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
		datetime& operator=(const datetime& source)
		{
			this->copy((datetime&)source);
			return *this;
		}
	};
};

#endif