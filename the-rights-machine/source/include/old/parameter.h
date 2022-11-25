#include "core/string/string.h"

#if !defined(__PARAMETER)
#define __PARAMETER

namespace web
{
	class parameter
	{
	public:
		string name;
		string value;

	public:
		parameter(string name = "", string value = "") 
		{ 
			this->name = name; 
			this->value = value; 
		}
		parameter(parameter const &source) { copy(source); }

		void copy(parameter const &source)
		{
			name = source.name;
			value = source.value;
		}

	public:
		parameter& operator=(const parameter& source)
		{
			this->copy((parameter&)source);
			return *this;
		}
	};
};

#endif