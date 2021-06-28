#include <gtest/gtest.h>
#include "test/client.h"
#include "test/responses.h"
#include "test/environment.h"

// test three, merged first two files into third new file
// test concurrent access

// test with two queries
// two different times

// more than 10 components

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

/*
TEST(BasicUnavailableQueryWithInMemoryQueue, BasicAssertions)
{
	long port = 5454;

	test::client client(string("test/data/basicUnavailable.json"));
	EXPECT_TRUE(client.initalised());

	web::page destination;

	EXPECT_TRUE(client.post("http://127.0.0.1", port, &destination));

	tests::data::responses responses(destination);
	EXPECT_TRUE(responses.initalised());
	EXPECT_TRUE(responses.data.size() == 1);

	tests::data::response response = responses.data.front();
	EXPECT_TRUE(response.status.compare(string("OK")) == 0);
	EXPECT_TRUE(response.available.compare(string("false")) == 0);
}
*/

TEST(BasicAvailableQueryWithInMemoryQueue, BasicAssertions)
{
	long port = 5454;

	test::client client(string("test/data/basicAvailable.json"));
	EXPECT_TRUE(client.initalised());

	web::page destination;
	
	EXPECT_TRUE(client.post("http://127.0.0.1", port, &destination));

	tests::data::responses responses(destination);
	EXPECT_TRUE(responses.initalised());
	EXPECT_TRUE(responses.data.size() == 1);

	tests::data::response response = responses.data.front();
	EXPECT_TRUE(response.status.compare(string("OK")) == 0);
	EXPECT_TRUE(response.available.compare(string("true")) == 0);
}


/*
TEST(BasicUnavailableQueryWithInMemoryQueue, BasicAssertions)
{
	long port = 5454;

	test::client client(string("test/data/outOfRange.json"));
	EXPECT_TRUE(client.initalised());

	web::page destination;

	EXPECT_TRUE(client.post("http://127.0.0.1", port, &destination));

	tests::data::responses responses(destination);
	EXPECT_TRUE(responses.initalised());
	EXPECT_TRUE(responses.data.size() == 1);

	tests::data::response response = responses.data.front();
	EXPECT_TRUE(response.status.compare(string("ERR")) == 0);
	EXPECT_TRUE(response.available.compare(string("false")) == 0);
}
*/
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
	::testing::AddGlobalTestEnvironment(new test::TestEnvironment);
    return RUN_ALL_TESTS();
}