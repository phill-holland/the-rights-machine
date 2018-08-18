#include "line.h"
#include "item.h"
#include <vector>
#include "../process/string.h"

#if !defined(__TEST_MESSAGE)
#define __TEST_MESSAGE

namespace tests
{
	namespace data
	{
		class message
		{
		public:
			string userID;
			string APIKey;

			std::vector<line> queries;
			std::vector<item> items;

		public:
			message()
			{
				userID = "1";
				APIKey = "1";
			}

			message(string userID, string APIKey)
			{
				this->userID = userID;
				this->APIKey = APIKey;
			}

			string json();
		};
	};
};

#endif