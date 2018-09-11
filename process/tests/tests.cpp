#include "stdafx.h"
#include "CppUnitTest.h"
#include <vector>
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
#include "../process/log.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace tests
{		
	TEST_CLASS(Tests)
	{
	public:
		
		TEST_METHOD(TestBasicQuery)
		{
			
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
			
			source.data(message.json());
			Log << "message\r\n"; 
			Log << message.json();
			
			server::starter starter;
			
			Assert::AreEqual(true, starter.initalised());
			Log << "starter.start\r\n";
			Assert::AreEqual(true, starter.start());
			Log << "starter.started\r\n";
			Assert::AreEqual(true, client.post(&destination, &source));// , L"Boom", LINE_INFO());
			
			data::response response;

			Assert::AreEqual(true, response.parse(destination.data()));
			Log << "response GUID\r\n";
			Log << response.GUID << "\r\n";
									
			int loops = 0;
			bool success = false;
			data::result result;

			while (loops < 5 && !success)
			{
				data::request request(message.userID, message.APIKey, response.GUID);
				source.data(request.json());

				Assert::AreEqual(true, client.get(&destination, &source));

				//data::result result;
				result.clear();

				Assert::AreEqual(true, result.parse(destination.data()));

				Log << "result GUID\r\n";
				Log << result.GUID << "\r\n";
				Log << result.status << "\r\n";
				Log << (result.available == true ? string("true") : string("false")) << "\r\n";

				success = result.status == data::result::STATUS::OK;

				if (!success) Sleep(1000);

				++loops;
			}
			
			Assert::AreEqual((int)data::result::STATUS::OK, (int)result.status);
			Log << "shutting down your arse\r\n";
			starter.shutdown();

			Log << "HERE\r\n";
			// decode destination, get GUID
			// send response request

			//Assert::AreEqual(1, 1);// name, mc.GetName());

			// todo
			// decode response
			// request result
			// setup errors
			// setup user talbes
			// setup database classes
			// error copy function
		}
	};
};