#include "core/string/string.h"
#include "core/custom/pair.h"
#include "parser/json/json.h"
#include "types/datetime.h"
#include "models/request.h"

#if !defined(__RESPONSE)
#define __RESPONSE

namespace data
{
	namespace response
	{		
		class response : public json::response::json
		{
		public:
			enum STATUS { ERR = 0, 
						  OK = 1, 
						  PENDING = 2, 
						  RANGE = 3,
						  UNKNOWN = 4 };

		public:
			string guid;
			string name;
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
			core::custom::pair pull(unsigned long index);

			bool validate(data::request::request &request);

		protected:
			string map(STATUS source);			
		};
	};
};

#endif