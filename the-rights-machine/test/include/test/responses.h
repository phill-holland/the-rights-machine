#include <boost/json.hpp>
#include "custom/string.h"
#include "response.h"
#include "page.h"
#include <vector>

#if !defined(__TEST_RESPONSES)
#define __TEST_RESPONSES

namespace tests
{
	namespace data
	{
		class responses
		{
			string key;
			response temp;
            bool init;

		public:
			std::vector<response> data;
			
		public:
			responses(web::page &source) { reset(source); }

            bool initalised() { return init; }
            void reset(web::page &source);            

		protected:
			void parse(const boost::json::value &value);
		};
	};	
};

#endif