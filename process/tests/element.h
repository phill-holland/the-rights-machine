#include "../process/string.h"

#if !defined(__ELEMENT)
#define __ELEMENT

namespace data
{
	class element
	{
	public:
		string value;

	public:
		element()
		{
			value = "value";
		}

		element(string value)
		{
			this->value = value;
		}

		string json();
	};
};

#endif