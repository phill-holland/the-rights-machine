#include "element.h"
#include <vector>
#include "../process/string.h"

#if !defined(__COMPONENT)
#define __COMPONENT

namespace data
{
	class component
	{
	public:
		string name;

		std::vector<element> elements;

	public:
		component()
		{
			name = "name";
		}

		component(string name)
		{
			this->name = name;
		}

		string json();
	};
};

#endif