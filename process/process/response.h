#include "string.h"
//#include "datetime.h"

#if !defined(__RESPONSE)
#define __RESPONSE

//using namespace global;

namespace data
{
	namespace response
	{
		// could just be a template class
		// with a pure virtual function to convert to JSON

		class response
		{
		public:
			string GUID;
			//int userID;
			//int responseID;
			//int messageID;
			//int itemID;
			//int code;
			//string description;
			//datetime created;
		};
	};
};

#endif