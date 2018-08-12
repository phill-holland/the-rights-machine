#include "line.h"
#include <vector>
#include "../process/string.h"

#if !defined(__ITEM)
#define __ITEM

namespace data
{
	class item
	{
	public:
		string name;

		std::vector<line> lines;

	public:
		item()
		{
			name = "name";
		}

		item(string name)
		{
			this->name = name;
		}

		string json();
	};
};

#endif