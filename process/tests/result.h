#include "../process/json.h"
#include "../process/pair.h"
#include "../process/string.h"
#include "../process/datetime.h"
#include "../process/comparison.h"
#include <unordered_map>

#if !defined(__TEST_RESULT)
#define __TEST_RESULT

namespace tests
{
	namespace data
	{
		using namespace global;

		class result : public ::data::json::request::json
		{
		public:
			enum STATUS { OK = 1, PENDING = 2, UNKNOWN = 3 };

			std::unordered_map<string, json *, comparison::hasher, comparison::equality> hash;

		public:
			string GUID;
			STATUS status;
			datetime created;
			bool available;

		public:
			result();

		public:
			bool add(custom::pair &source);

			int identity() { return 0; }
			void clear()  { }
			string identifier() { return string("response"); }

		public:
			json *find(string FQDN);
		};
	};	
};

#endif