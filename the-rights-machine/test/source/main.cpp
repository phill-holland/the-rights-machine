#include <gtest/gtest.h>
#include "test/client.h"
#include "test/responses.h"
#include "test/environment.h"

const long port = 5454;
const string url("http://127.0.0.1");

TEST(BasicUnavailableQueryWithInMemoryQueue, BasicAssertions)
{
	test::client client(string("test/data/basicUnavailable.json"));
	EXPECT_TRUE(client.initalised());

	net::web::page destination;

	EXPECT_TRUE(client.post(url, port, &destination));

	tests::data::responses responses(destination);

	EXPECT_TRUE(responses.initalised());
	EXPECT_TRUE(responses.data.size() == 1);

	tests::data::response response = responses.data.front();
	EXPECT_TRUE(response.status.compare(string("OK")) == 0);
	EXPECT_TRUE(response.available.compare(string("false")) == 0);
}

TEST(BasicAvailableQueryWithInMemoryQueue, BasicAssertions)
{
	test::client client(string("test/data/basicAvailable.json"));
	EXPECT_TRUE(client.initalised());

	net::web::page destination;
	
	EXPECT_TRUE(client.post(url, port, &destination));

	tests::data::responses responses(destination);
	EXPECT_TRUE(responses.initalised());
	EXPECT_TRUE(responses.data.size() == 1);

	tests::data::response response = responses.data.front();
	EXPECT_TRUE(response.status.compare(string("OK")) == 0);
	EXPECT_TRUE(response.available.compare(string("true")) == 0);
}

TEST(BasicOutOfRangeQueryWithInMemoryQueue, BasicAssertions)
{
	test::client client(string("test/data/outOfRange.json"));
	EXPECT_TRUE(client.initalised());

	net::web::page destination;

	EXPECT_TRUE(client.post(url, port, &destination));

	tests::data::responses responses(destination);
	EXPECT_TRUE(responses.initalised());
	EXPECT_TRUE(responses.data.size() == 1);

	tests::data::response response = responses.data.front();
	EXPECT_TRUE(response.status.compare(string("RANGE")) == 0);
	EXPECT_TRUE(response.available.compare(string("false")) == 0);
}

TEST(InvalidJsonWithInMemoryQueue, BasicAssertions)
{
	test::client client(string("test/data/invalidJson.json"));
	EXPECT_TRUE(client.initalised());

	net::web::page destination;

	EXPECT_TRUE(client.post(url, port, &destination));

	tests::data::responses responses(destination);
	EXPECT_TRUE(responses.initalised());
	EXPECT_TRUE(responses.data.size() == 1);

	tests::data::response response = responses.data.front();
	EXPECT_TRUE(response.status.compare(string("ERR")) == 0);
	EXPECT_TRUE(response.available.compare(string("false")) == 0);
}

TEST(TwoItemsBothAvailableQueryWithInMemoryQueue, BasicAssertions)
{
	test::client client(string("test/data/twoItemsBothAvailable.json"));
	EXPECT_TRUE(client.initalised());

	net::web::page destination;

	EXPECT_TRUE(client.post(url, port, &destination));

	tests::data::responses responses(destination);
	EXPECT_TRUE(responses.initalised());
	EXPECT_TRUE(responses.data.size() == 2);

	tests::data::response response = responses.data[0];
	EXPECT_TRUE(response.status.compare(string("OK")) == 0);
	EXPECT_TRUE(response.available.compare(string("true")) == 0);

	response = responses.data[1];
	EXPECT_TRUE(response.status.compare(string("OK")) == 0);
	EXPECT_TRUE(response.available.compare(string("true")) == 0);
}

TEST(TwoItemsOneAvailableQueryWithInMemoryQueue, BasicAssertions)
{
	test::client client(string("test/data/twoItemsOneAvailable.json"));
	EXPECT_TRUE(client.initalised());

	net::web::page destination;

	EXPECT_TRUE(client.post(url, port, &destination));

	tests::data::responses responses(destination);
	EXPECT_TRUE(responses.initalised());
	EXPECT_TRUE(responses.data.size() == 2);

	tests::data::response response = responses.data[0];
	EXPECT_TRUE(response.status.compare(string("RANGE")) == 0);
	EXPECT_TRUE(response.available.compare(string("false")) == 0);

	response = responses.data[1];
	EXPECT_TRUE(response.status.compare(string("OK")) == 0);
	EXPECT_TRUE(response.available.compare(string("true")) == 0);
}

TEST(basicUnavailableMinusComponentQueryWithInMemoryQueue, BasicAssertions)
{
	test::client client(string("test/data/basicUnavailableMinusComponent.json"));
	EXPECT_TRUE(client.initalised());

	net::web::page destination;

	EXPECT_TRUE(client.post(url, port, &destination));

	tests::data::responses responses(destination);
	EXPECT_TRUE(responses.initalised());
	EXPECT_TRUE(responses.data.size() == 1);

	tests::data::response response = responses.data.front();
	EXPECT_TRUE(response.status.compare(string("OK")) == 0);
	EXPECT_TRUE(response.available.compare(string("false")) == 0);
}

TEST(basicAvailableWithFullISODateWithInMemoryQueue, BasicAssertions)
{
	test::client client(string("test/data/basicWithFullISODateAvailable.json"));
	EXPECT_TRUE(client.initalised());

	net::web::page destination;

	EXPECT_TRUE(client.post(url, port, &destination));

	tests::data::responses responses(destination);
	EXPECT_TRUE(responses.initalised());
	EXPECT_TRUE(responses.data.size() == 1);

	tests::data::response response = responses.data.front();
	EXPECT_TRUE(response.status.compare(string("OK")) == 0);
	EXPECT_TRUE(response.available.compare(string("true")) == 0);
}

TEST(basicAvailableWithBadDatesWithInMemoryQueue, BasicAssertions)
{
	test::client client(string("test/data/basicWithBadDatesAvailable.json"));
	EXPECT_TRUE(client.initalised());

	net::web::page destination;

	EXPECT_TRUE(client.post(url, port, &destination));

	tests::data::responses responses(destination);
	EXPECT_TRUE(responses.initalised());
	EXPECT_TRUE(responses.data.size() == 1);

	tests::data::response response = responses.data.front();
	EXPECT_TRUE(response.status.compare(string("ERR")) == 0);
	EXPECT_TRUE(response.available.compare(string("false")) == 0);
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
	::testing::AddGlobalTestEnvironment(new test::TestEnvironment);
    return RUN_ALL_TESTS();
}
