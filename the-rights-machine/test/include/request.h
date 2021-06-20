#include "line.h"
#include "item.h"
#include <vector>
#include "custom/string.h"

#if !defined(__TEST_REQUEST)
#define __TEST_REQUEST

namespace tests
{
	namespace data
	{
		class request
		{
		public:
			string user;
			string APIKey;
			string GUID;

		public:
			request()
			{
				user = "1";
				APIKey = "1";
				GUID = "1";
			}

			request(string user, string APIKey, string GUID)
			{
				this->user = user;
				this->APIKey = APIKey;
				this->GUID = GUID;
			}

			string json();
		};
	};
};

#endif