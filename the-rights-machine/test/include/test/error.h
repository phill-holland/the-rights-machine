#include "parser/json/legacy/json.h"
#include "core/custom/pair.h"
#include "core/string/string.h"
#include "core/string/comparison.h"
#include <unordered_map>

#if !defined(__TEST_RESULT)
#define __TEST_RESULT

namespace tests
{
	namespace data
	{
		class error : public json::request::json
		{
			std::unordered_map<string, json *, comparison::hasher, comparison::equality> hash;

		public:
			long code;
			string name;
			string description;

		public:
			error();

		public:
			bool add(core::custom::pair source);

			int identity() { return 0; }
			void clear()  { }
			string identifier() { return string("error"); }

		public:
			json *find(string FQDN);
		};
	};
};

#endif