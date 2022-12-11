#include "core/string/string.h"
#include "core/custom/pair.h"
#include "parser/json/legacy/json.h"
#include "core/custom/datetime.h"
#include "models/request.h"

#if !defined(__RESPONSE)
#define __RESPONSE

namespace models
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
			core::custom::datetime created;
			bool available;

		public:
			response() { clear(); }
			response(STATUS status) { clear(); this->status = status; }

			string identifier() { return string("response"); }

			void clear();

			unsigned long pairs();
			core::custom::pair pull(unsigned long index);

			bool validate(models::request::request &request);

		protected:
			string map(STATUS source);			
		};
	};
};

#endif