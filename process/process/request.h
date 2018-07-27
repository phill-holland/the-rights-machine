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
			int requestID;
			string GUID;
			int userID;

		public:
			request() { clear(); }
			request(json *parent) { clear(); json::parent(parent); }
			request(request const &source) { clear(); copy(source); }

			int identity() { return requestID; }

			void clear();
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