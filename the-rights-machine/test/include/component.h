#include "element.h"
#include <vector>
#include "custom/string.h"

#if !defined(__TEST_COMPONENT)
#define __TEST_COMPONENT

namespace tests
{
	namespace data
	{
		class component
		{
		public:
			string name;

			std::vector<element> elements;

		public:
			component()
			{
				name = "name";
			}

			component(string name)
			{
				this->name = name;
			}

			string json();
		};
	};
};

#endif