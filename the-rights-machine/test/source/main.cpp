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

// NEED TO TEST FOR BAD JSON
// TEST WITH TWO CNOCURRENT REQUESTS,
// TEST WITH TWO ITEMS, SINGLE QUERY
// AVAILABLE and AVAILABLE, 
// AVAILABLE and UNAVAILABLE, 
// UNAVAILABLE and AVAILABLE, 
// UNAVAILABLE and UNAVAILABLE
// NO ACQUIRED RIGHTS TEST
// 15 COMPONENTS TEST

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

TEST(BasicOutOfRangeQueryWithInMemoryQueue, BasicAssertions)
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
	EXPECT_TRUE(response.status.compare(string("RANGE")) == 0);
	EXPECT_TRUE(response.available.compare(string("false")) == 0);
}


/*
// two items, same query, both available
TEST(BasicUnavailableQueryWithInMemoryQueue, BasicAssertions)
{
	long port = 5454;

	test::client client(string("test/data/twoItemsBothAvailable.json"));
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

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
	::testing::AddGlobalTestEnvironment(new test::TestEnvironment);
    return RUN_ALL_TESTS();
}