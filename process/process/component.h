#include "string.h"

#if !defined(__COMPONENT)
#define __COMPONENT

namespace data
{
	class component
	{
	public:
		int componentID;
		int lineID;
		string name;
	};
};

#endif