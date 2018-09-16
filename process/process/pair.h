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
		pair(pair const &source) { copy(source); }

		void copy(pair const &source)
		{
			name = source.name;
			value = source.value;
		}

	public:
		pair& operator=(const pair& source)
		{
			this->copy((pair&)source);
			return *this;
		}
	};
	
};

#endif