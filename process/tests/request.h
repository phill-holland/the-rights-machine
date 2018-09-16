#include "line.h"
#include "item.h"
#include <vector>
#include "../process/string.h"

#if !defined(__TEST_REQUEST)
#define __TEST_REQUEST

namespace tests
{
	namespace data
	{
		class request
		{
		public:
			string APIKey;
			string GUID;

		public:
			request()
			{
				APIKey = "1";
				GUID = "1";
			}

			request(string APIKey, string GUID)
			{
				this->APIKey = APIKey;
				this->GUID = GUID;
			}

			string json();
		};
	};
};

#endif