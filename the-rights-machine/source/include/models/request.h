#include "core/string/string.h"
#include "core/custom/pair.h"
#include "parser/json/json.h"
#include "types/datetime.h"

#if !defined(__REQUEST)
#define __REQUEST

namespace data
{
	namespace request
	{
		class request : public ::json::request::json
		{
		public:
			int requestID;
			string apikey;
			string guid;
			string user;
			datetime created;

		public:
			request() { clear(); }
			request(json *parent) { clear(); json::parent(parent); }
			request(request const &source) { clear(); copy(source); }

			int identity() { return requestID; }

			void clear();

			::json::request::json *find(string FQDN);

			void copy(request const &source);

		public:
			string output();

		public:
			string identifier() { return string("REQUEST"); }
			bool add(core::custom::pair source);

		public:
			request& operator=(const request& source)
			{
				this->copy((request&)source);
				return *this;
			}
		};
	};
};

#endif