#include "stdafx.h"
#include "response.h"
#include "../process/charbuf.h"
#include "../process/crumbs.h"
#include "../process/log.h"

tests::data::response::response()
{
	json *identifiers[] = { this };

	for (long i = 0L; i < 1L; ++i)
	{
		Log << "ADD " << identifiers[i]->FQDN() << "\r\n";
		hash[identifiers[i]->FQDN()] = identifiers[i];
	}
}

bool tests::data::response::add(custom::pair &source)
{
	Log << "boobies " << source.name << "\r\n";
	if (string("guid").icompare(source.name))
	{
		GUID = source.value;
		return true;
	}

	return false;
}

::data::json::request::json *tests::data::response::find(string FQDN)
{
	Log << "FQDN " << FQDN << "\r\n";
	std::unordered_map<string, ::data::json::request::json*, comparison::hasher, comparison::equality>::iterator i = hash.find(FQDN);
	if (i != hash.end()) return (::data::json::request::json*)i->second;

	Log << "RETURN NULL\r\n";
	return NULL;
}

bool tests::data::response::parse(string json)
{
	/*{"response" : 
{
"GUID" : "91B72F30-1C3D-4DB9-9957-9213ED277199"
}
}*/

	bool left = true, quotes = false;
	long brackets = 0L, squares = 0L;

	global::charbuf label, value;
	crumbs::crumbs parents;

	for (long i = 0L; i < json.count(); ++i)
	{
		if (json[i] == '"')
		{
			quotes = !quotes;
		}
		else
		{
			if (!quotes)
			{
				if (json[i] == '{')
				{
					//string t = (string)label;
					//Log << "label :" << t << "\r\n";

					if (!parents.push((string)label))
					{
						return false;
						//error(string("NESTING_TOO_DEEP"));
					}

					label.clear();
					value.clear();

					left = true;

					++brackets;
				}
				else if (json[i] == '}')
				{
					//if (get() == MODE::POST)
					//{
					if (!parents.isempty())
					{
						::data::json::request::json *current = find(parents.FQDN());
						if (current != NULL) current->add(custom::pair(label, value));
					}

					//queue::base *b = task.message.findQ(parents.FQDN());
					//if (b != NULL) b->flush();

					//Log << parents.FQDN() << "\r\n";
					//Log << task.message.items.FQDN() << "\r\n\r\n";
					/*
					if (parents.FQDN().icompare(task.message.items.FQDN()))
					{
					//Log << "PUSH MESSAGE TO OUTPUT\r\n";
					//Log << "NEED TO WRITE OUTPUT FUNCTION FOR MESSAGE\r\n";

					//guid::guid g;
					//task.message.GUID = g.get();
					t//ask.message.created = global::datetime::now();
					// PUSH GUID TO UNORDERED_MAP

					task.message.output();

					if (!c->manager->set(task))
					{
					Log << "server error\r\n";
					error(string("MESSAGE_PUSH"));
					}
					else
					{
					Log << "response OK\r\n";

					::data::response::response response;
					// need different type of structure!!!
					response.GUID = task.message.GUID;
					response.userID = 1;
					response.created = datetime::now();
					response.queryID = 1;
					response.available = false;

					outputter.set(&response);
					// build response list - json classes
					// when content-length is read, write response
					}
					}*/
					//}

					left = true;

					Log << "label " << (string)label << " value " << (string)value << "\r\n";
					label.clear();
					value.clear();

					if (!parents.pop())
					{
						return false;
						//error(string("BRACKET_MISMATCH"));
					}

					--brackets;
					if (brackets < 0)
					{
						return false;
						//error(string("BRACKET_MISMATCH"));
					}
				}
				else if (json[i] == '[')
				{
					if (!parents.push(label))
					{
						return false;
						//error(string("NESTING_TOO_DEEP"));
					}

					label.clear();
					value.clear();

					left = true;
					++squares;
				}
				else if (json[i] == ']')
				{
					left = true;

					label.clear();
					value.clear();

					if (!parents.pop())
					{
						return false;
						//error(string("BRACKET_MISMATCH"));
					}

					--squares;
					if (squares < 0)
					{
						return false;
						//error(string("BRACKET_MISMATCH"));
					}
				}
				else if (json[i] == ':')
				{
					left = false;
				}
				else if (json[i] == ',')
				{
					//if (get() == MODE::POST)
					//{
					if (!parents.isempty())
					{
						::data::json::request::json *current = find(parents.FQDN());
						if (current != NULL) current->add(custom::pair(label, value));
					}
					//}


					left = true;

					label.clear();
					value.clear();
				}
				else if ((json[i] >= '0') && (json[i] <= '9'))
				{
					if (!left)
					{
						if (!value.push(json[i]))
						{
							return false;
							//error(string("STRING_TOO_LONG"));
						}
					}
				}
			}
			else
			{
				if (brackets >= 1)
				{
					if ((quotes && left))
					{
						if (!label.push(json[i]))
						{
							return false;
							//error(string("STRING_TOO_LONG"));
						}
					}
					else if ((quotes && !left))
					{
						if (!value.push(json[i]))
						{
							return false;
							//error(string("STRING_TOO_LONG"));
						}
					}
				}
			}
		}
	}

	return true;
}
