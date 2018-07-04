#include "string.h"

#if !defined(__ITEM)
#define __ITEM

namespace data
{
	class item
	{
	public:
		int itemID;
		int messageID;
		string name;
	};
};

#endif