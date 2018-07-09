#include "string.h"

#if !defined(__PAIR)
#define __PAIR

namespace custom
{
	class pair
	{
	public:
		string name;	
		string value;

	public:
		pair() { name = ""; value = ""; }
		pair(string n, string v) { name = n; value = v; }
	};
};

#endif