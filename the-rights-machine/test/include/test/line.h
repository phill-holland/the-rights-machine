#include "component.h"
#include <vector>
#include "custom/string.h"
#include "datetime.h"

#if !defined(__TEST_LINE)
#define __TEST_LINE

namespace tests
{
	namespace data
	{
		class line
		{
		public:
			global::datetime start, end;
			string exclusivity;
			string type;

			std::vector<component> components;

		public:
			line()
			{
				start = global::datetime(2018, 01, 01);
				end = global::datetime(2019, 01, 01);
				exclusivity = "0";
				type = "0";
			}

			line(global::datetime start, global::datetime end)
			{
				this->start = start;
				this->end = end;
			}

			string json();
		};
	};
};

#endif