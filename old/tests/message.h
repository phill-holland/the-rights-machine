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
			string user;
			string APIKey;

			std::vector<line> queries;
			std::vector<item> items;

		public:
			message()
			{
				user = "1";
				APIKey = "1";
			}

			message(string user, string APIKey)
			{
				this->user = user;
				this->APIKey = APIKey;
			}

			string json();
		};
	};
};

#endif