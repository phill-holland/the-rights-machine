#include <gtest/gtest.h>
#include "test/client.h"
#include "messaging.h"
#include "starter.h"

TEST(BasicQueryWithInMemoryQueue, BasicAssertions)
{
	// add to launch.json for debugging breakpoints
	// set CWD so that data is loaded correctly
	// create configuration.json
	// copy debugger.cpp into project
	// add database SQL to project
	// guid create on ubuntu
	test::client client(string("data/body.json"));
	messaging::memory::memory messaging;
	server::starter starter(&messaging);
	
	EXPECT_TRUE(starter.initalised());
	EXPECT_TRUE(starter.start());
	// sleep here??

	client.post("https://localhost", 5555);
	// load body file
	// issue http query
}

/*
TEST(DateComparionsTests, BasicAssertions) 
{
	global::datetime a(2018, 10, 2);
	global::datetime b(2017, 10, 2);
	global::datetime c(2018, 9, 2);
	global::datetime d(2018, 10, 1);
	global::datetime e(2018, 10, 3);
	global::datetime f(2018, 11, 2);
	global::datetime g(2019, 9, 2);

	EXPECT_FALSE(a < b);
	EXPECT_TRUE(b < a);
	EXPECT_FALSE(a < a);
	EXPECT_TRUE(c < a);
	EXPECT_TRUE(d < a);
	EXPECT_FALSE(e < a);
	EXPECT_FALSE(f < a);
	EXPECT_FALSE(g < a);
}
*/
/*
#include <gtest/gtest.h>

// Demonstrate some basic assertions.
TEST(HelloTest, BasicAssertions) {
  // Expect two strings not to be equal.
  EXPECT_STRNE("hello", "world");
  // Expect equality.
  EXPECT_EQ(7 * 6, 42);
}
*/
int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}