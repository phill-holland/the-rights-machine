#include "string.h"
#include "datetime.h"

#if !defined(__LINE)
#define __LINE

using namespace global;

namespace data
{
	class line
	{
	public:
		int lineID;
		int itemID;
		datetime start;
		datetime end;
		int exclusivityID;
		int typeID;
	};
};

#endif