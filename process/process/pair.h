#include "string.h"

#if !defined(__PAIR)
#define __PAIR

namespace custom
{/*
	namespace base
	{
		template <class X> class pair
		{
		public:
			X name;
			X value;

		public:
			//pair() { name = ""; value = ""; }
			pair() { }
			pair(X name, X value) { this->name = name; this->value = value; }
		};
	};

	class pair : public base::pair<string> 
	{ 
	public:
		pair() { name = ""; value = ""; }
		pair(string name, string value) : base::pair<string>(name, value) { }
	};*/
	
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