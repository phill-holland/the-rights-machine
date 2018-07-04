#include "string.h"
#include "datetime.h"

#if !defined(__RESPONSE)
#define __RESPONSE

using namespace global;

namespace data
{
	class response
	{
	public:
		int responseID;
		int itemID;
		int code;
		string description;
		datetime created;
	};
};

#endif