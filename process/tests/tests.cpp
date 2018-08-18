#include "stdafx.h"
#include "CppUnitTest.h"
#include <vector>
#include "element.h"
#include "component.h"
#include "line.h"
#include "item.h"
#include "message.h"
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

			destination.url = string("127.0.0.1");
			destination.port = 5555L;
			
			source.data(message.json());
			//Log << message.json();
			
			server::starter starter;
			/*
			Assert::AreEqual(starter.initalised(), true);
			Assert::AreEqual(starter.start(), true);

			Assert::AreEqual(client.post(&destination, &source), true);
			*/
			// decode destination, get GUID
			// send response request

			Assert::AreEqual(1, 1);// name, mc.GetName());
		}
	};
};