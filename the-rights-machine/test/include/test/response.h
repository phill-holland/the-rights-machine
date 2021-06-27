#include "custom/string.h"
#include "datetime.h"
#include "comparison.h"
#include <unordered_map>

#if !defined(__TEST_RESPONSE)
#define __TEST_RESPONSE

namespace tests
{
	namespace data
	{
		using namespace global;

		class response
		{
		public:
			string GUID;
			string status;
			string available;
			datetime created;
			
		public:
			response() { }
		};
	};	
};

#endif