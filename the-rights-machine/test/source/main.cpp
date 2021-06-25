//#include "bsttst.h"

#include <gtest/gtest.h>
#include "test/client.h"
#include "messaging.h"
#include "settings.h"
#include "starter.h"
//#include "bsttst.h"

/*
TEST(JsonCustomStream, BasicAssertions)
{
	bsttst::bsttst moo;

// convert read json back into JSON, and then compare with input JSON file!
	moo.go(string("test/data/body.json"));
	moo.output();

}
*/

TEST(BasicQueryWithInMemoryQueue, BasicAssertions)
{
	// BODY.JSON should return not available.
	
	// add to launch.json for debugging breakpoints OK
	// set CWD so that data is loaded correctly OK
	// create configuration.json
	// copy debugger.cpp into project
	// add database SQL to project
	// guid create on ubuntu SORT OF OK
	// add error responses to log file (during reset() stages)
	// need a thread safe std::vector
	// return errors to web client

	long port = 5454;

	test::client client(string("test/data/basicUnavailable.json"));
	EXPECT_TRUE(client.initalised());

	messaging::memory::memory messaging;
	server::settings setup(&messaging, port);
	server::starter starter(setup);

	EXPECT_TRUE(starter.initalised());
	EXPECT_TRUE(starter.start());
	// sleep here??
	//std::this_thread::sleep_for(std::chrono::milliseconds(5000));

	client.post("http://127.0.0.1", port);

		//std::this_thread::sleep_for(std::chrono::milliseconds(15000));

	// load body file
	// issue http query
}

// 15 component test
// multiple programme test
// not aviable first test
// second available test

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