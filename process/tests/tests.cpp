#include "stdafx.h"
#include "CppUnitTest.h"
#include <vector>
#include "../process/starter.h"
#include "../process/string.h"
#include "../process/http.h"
#include "../process/page.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace tests
{		
	TEST_CLASS(Tests)
	{
	public:
		
		TEST_METHOD(TestBasicQuery)
		{
			//std::string name = "Bill";
			//MyClass mc(name);
			Assert::AreEqual(1, 1);// name, mc.GetName());
		}
	};
};