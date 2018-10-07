#include "string.h"
#include "json.h"
#include "datetime.h"
#include "request.h"

#if !defined(__RESPONSE)
#define __RESPONSE

namespace data
{
	namespace response
	{
		using namespace global;

		class response : public data::json::response::json
		{
		public:
			enum STATUS { ERR = 0, OK = 1, PENDING = 2, UNKNOWN = 3 };

		public:
			string guid;
			string user;
			STATUS status;
			datetime created;
			bool available;

		public:
			response() { clear(); }
			response(STATUS status) { clear(); this->status = status; }

			string identifier() { return string("response"); }

			void clear();

			unsigned long pairs();
			custom::pair pull(unsigned long index);

			bool validate(data::request::request &request);

		protected:
			string map(STATUS source);			
		};
	};
};

#endif