#include "string.h"
#include "json.h"
#include "datetime.h"

#if !defined(__RESPONSE)
#define __RESPONSE

//using namespace global;

namespace data
{
	namespace response
	{
		using namespace global;

		// could just be a template class
		// with a pure virtual function to convert to JSON
				
		class response : public data::json::response::json
		{
		public:
			string GUID;
			int userID;
			//string status;
			datetime created;

			int queryID;
			bool available;

			// put error::type class here
			// default to none

			// for each error/responds pushed onto response queuer

		public:
			string identifier() { return string("response"); }

			unsigned long pairs();
			custom::pair& pull(unsigned long index);

			//int userID;
			//int responseID;
			//int messageID;
			//int itemID;
			//int code;
			//string description;
			//datetime created;

			// errors type data class, inherited from allocator
			// possible error queue destination
		};
	};
};

#endif