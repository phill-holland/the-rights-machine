#include "core/string/string.h"
#include "core/custom/datetime.h"

#if !defined(__TEST_RESPONSE)
#define __TEST_RESPONSE

namespace tests
{
	namespace data
	{
		class response
		{
		public:
			string GUID;
			string name;
			string status;
			string available;
			core::custom::datetime created;
			
		public:
			response() { }
		};
	};	
};

#endif