#include "string.h"
#include "datetime.h"

#if !defined(__MESSAGE)
#define __MESSAGE

using namespace global;

namespace data
{
	class message
	{
	public:
		int messageID;
		int userID;
		string GUID;
		datetime created;
		datetime finished;		
	};
};

#endif