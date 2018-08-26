#include "../process/string.h"
#include "../process/json.h"
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
			std::unordered_map<string, ::data::json::request::json *, comparison::hasher, comparison::equality> hash;

		public:
			string GUID;

		public:
			response();

		public:
			bool parse(string json);

		public:
			int identity() { return 0; }
			void clear() { }
			string identifier() { return string("response"); }
			bool add(custom::pair &source);

		public:
			::data::json::request::json *find(string FQDN);
		};
	};
};

#endif