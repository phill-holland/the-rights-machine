#include "../process/json.h"
#include "../process/string.h"
#include "../process/comparison.h"
#include <unordered_map>

#if !defined(__TEST_RESPONSE)
#define __TEST_RESPONSE

namespace tests
{
	namespace data
	{
		class response : public ::data::json::request::json
		{
			std::unordered_map<string, json *, comparison::hasher, comparison::equality> hash;

		public:
			string GUID;

		public:
			response();

		public:
			bool add(custom::pair source);

			int identity() { return 0; }
			void clear()  { }
			string identifier() { return string("response"); }

		public:
			json *find(string FQDN);
		};
	};
};

#endif