#include "custom/string.h"

#if !defined(__TEST_ELEMENT)
#define __TEST_ELEMENT

namespace tests
{
	namespace data
	{
		class element
		{
		public:
			string value;

		public:
			element()
			{
				value = "value";
			}

			element(string value)
			{
				this->value = value;
			}

			string json();
		};
	};
};

#endif