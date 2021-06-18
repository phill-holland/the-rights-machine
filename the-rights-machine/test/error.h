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

		class error : public ::data::json::request::json
		{
			std::unordered_map<string, json *, comparison::hasher, comparison::equality> hash;

		public:
			long code;
			string name;
			string description;

		public:
			error();

		public:
			bool add(custom::pair source);

			int identity() { return 0; }
			void clear()  { }
			string identifier() { return string("error"); }

		public:
			json *find(string FQDN);
		};
	};
};

#endif