#include "string.h"
#include "json.h"

#if !defined(__REQUEST)
#define __REQUEST

namespace data
{
	namespace request
	{
		class request : public json::request::json
		{
		public:
			//string requestID;
			int requestID;
			string GUID;
			string user;

		public:
			request() { clear(); }
			request(json *parent) { clear(); json::parent(parent); }
			request(request const &source) { clear(); copy(source); }

			int identity() { return requestID; }

			void clear();

			data::json::request::json *find(string FQDN);

			void copy(request const &source);

		public:
			void output();

		public:
			string identifier() { return string("REQUEST"); }
			bool add(custom::pair &source);

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