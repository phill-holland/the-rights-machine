#include "stdafx.h"
#include "CppUnitTest.h"
#include "../process/wsock.h"
#include <windows.h>
#include "element.h"
#include "component.h"
#include "line.h"
#include "item.h"
#include "message.h"
#include "response.h"
#include "request.h"
#include "result.h"
#include "../process/starter.h"
#include "../process/string.h"
#include "../process/http.h"
#include "../process/page.h"
#include "../process/message.h"
#include "../process/storage.h"
#include "../process/log.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace tests
{		
	// to do
	// error json responses - send to client, log in database
	// (including error strings)

	// database classes

	// pending class, self tidy up

	// server, responses, self tidy up

	// av. algorithm, check split results (iterate through all returned structures, if all matches, combine date ranges

	// GPU implementation

	// av. algorithm, add request mode, to wait for result
	// av.algorithm, add to request json, endpoint to post result

	TEST_CLASS(Tests)
	{
	public:
		
		TEST_METHOD(TestBasicQuery)
		{
			string location = "DRIVER=SQL Server Native Client 11.0;SERVER=DESKTOP-DHP798L;UID=sa;PWD=Funjuice97?;WSID=MSSQLSERVER;DATABASE=Process;";

			data::component country(string("country"));
			data::component language(string("language"));
			
			country.elements.push_back(data::element("england"));
			country.elements.push_back(data::element("france"));

			language.elements.push_back(data::element("english"));
			language.elements.push_back(data::element("french"));

			data::line line;

			line.components.push_back(country);
			line.components.push_back(language);

			data::item item(string("item1"));
			item.lines.push_back(line);

			data::line query;

			query.components.push_back(country);
			query.components.push_back(language);

			data::message message;
			
			message.queries.push_back(query);
			message.items.push_back(item);
			
			web::page source, destination;			
			http::client::client client;

			destination.url = string("http://127.0.0.1");
			destination.port = 5555L;
			
			message.user = (string)guid::guid(string("5CEF8B8F-98BB-4CC1-9A1F-ECF01F1EF255"));
			message.APIKey = (string)guid::guid(string("FF4ABFA3-B7CE-457F-B827-6EC7AFBEC7BC"));

			source.data(message.json());
					
			messaging::memory::memory messages;

			server::starter starter(&messages);// location);

			Assert::AreEqual(true, starter.initalised());
			Assert::AreEqual(true, starter.start());

			Assert::AreEqual(true, client.post(&destination, &source));
			
			data::response response;

			Assert::AreEqual(true, response.parse(destination.data()));
									
			int loops = 0;
			bool success = false;
			data::result result;

			while (loops < 5 && !success)
			{
				data::request request(message.user, message.APIKey, response.GUID);
				source.data(request.json());

				Assert::AreEqual(true, client.get(&destination, &source));

				result.clear();

				Assert::AreEqual(true, result.parse(destination.data()));

				success = result.status == data::result::STATUS::OK;

				if (!success) Sleep(1000);

				++loops;
			}
						
			starter.shutdown();

			Assert::AreEqual((int)data::result::STATUS::OK, (int)result.status);
			Assert::AreEqual(true, result.available);

			// todo
			// add user verifications table
			// install vs 2017
			// create jquery/ajax test page
			// registration process
			// decode response
			// request result
			// setup errors
			// setup user talbes
			// setup database classes
			// error copy function

			// test connect with valid user
			// withuot valid user

			// test database mode
		}
	
		TEST_METHOD(TestDatabaseQuery)
		{
			// this isn't a finished test, does not communication with server/cpu class

			string location = "DRIVER=SQL Server Native Client 11.0;SERVER=DESKTOP-DHP798L;UID=sa;PWD=Funjuice97?;WSID=MSSQLSERVER;DATABASE=Process;";

			data::component country(string("country"));
			data::component language(string("language"));

			country.elements.push_back(data::element("england"));
			country.elements.push_back(data::element("france"));

			language.elements.push_back(data::element("english"));
			language.elements.push_back(data::element("french"));

			data::line line;

			line.components.push_back(country);
			line.components.push_back(language);

			data::item item(string("item1"));
			item.lines.push_back(line);

			data::line query;

			query.components.push_back(country);
			query.components.push_back(language);

			data::message message;

			message.queries.push_back(query);
			message.items.push_back(item);

			::data::message::message msg;
			msg.parse(message.json());

			msg.guid = (string)guid::guid(string("5CEF8B8F-98BB-4CC1-9A1F-ECF01F1EF255"));
			msg.user = (string)guid::guid();
			msg.apikey = (string)guid::guid(string("FF4ABFA3-B7CE-457F-B827-6EC7AFBEC7BC"));
			msg.created = global::datetime::now();
			msg.finished = global::datetime::now();

			database::odbc::factory::connection *connections = new database::odbc::factory::connection();
			Assert::AreEqual(true, connections != NULL);
			Assert::AreEqual(true, connections->initalised());

			database::odbc::factory::recordset *recordsets = new database::odbc::factory::recordset();
			Assert::AreEqual(true, recordsets != NULL);
			if (!recordsets->initalised()) return;

			database::settings settings(location, connections, recordsets);
			Assert::AreEqual(true, settings.initalised());

			database::storage::message storage;

			storage.open(settings);

			Assert::AreEqual(true, storage.write(msg));

			storage.close();

			delete recordsets;
			delete connections;
		}

		TEST_METHOD(TestInvalidUser)
		{
			// to do
			// query implement to/from database
			// test loading message from database
			// clear message from database after read
			// test user validation


			// test error logging to database
			// write error long descriptions

			// with insert into errors table

			// create watchdog, which writes every five minutes to DB table
			// or create "PING" command in server itself

			// write to database field every X seconds

			// photoshop plug in??
		}
	};
};