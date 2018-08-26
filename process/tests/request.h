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
			string userID;
			string APIKey;
			string GUID;

		public:
			request()
			{
				userID = "1";
				APIKey = "1";
				GUID = "1";
			}

			request(string userID, string APIKey, string GUID)
			{
				this->userID = userID;
				this->APIKey = APIKey;
				this->GUID = GUID;
			}

			string json();
		};
	};
};

#endif