#include "parser/json/legacy/json.h"
#include "parser/json/legacy/charbuf.h"
#include "parser/json/crumbs.h"
#include <vector>

using namespace parser::json::legacy;

bool json::request::json::parse(string json)
{
	bool left = true, quotes = false;
	long brackets = 0L, squares = 0L;

	charbuf label, value;
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
					if (!parents.push((string)label))
					{
						return false;
					}

					label.clear();
					value.clear();

					left = true;

					++brackets;
				}
				else if (json[i] == '}')
				{
					if (!parents.isempty())
					{
						::json::request::json *current = find(parents.FQDN());
						if (current != NULL) current->add(core::custom::pair(label, value));

						queue::base *b = findQ(parents.FQDN());
						if (b != NULL) b->flush();
					}

					left = true;

					label.clear();
					value.clear();

					if (!parents.pop())
					{
						return false;
					}

					--brackets;
					if (brackets < 0)
					{
						return false;
					}
				}
				else if (json[i] == '[')
				{
					if (!parents.push(label))
					{
						return false;
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
					}

					--squares;
					if (squares < 0)
					{
						return false;
					}
				}
				else if (json[i] == ':')
				{
					left = false;
				}
				else if (json[i] == ',')
				{
					if (!parents.isempty())
					{
						::json::request::json *current = find(parents.FQDN());
						if (current != NULL) current->add(core::custom::pair(label, value));
					}

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
						}
					}
					else if ((quotes && !left))
					{
						if (!value.push(json[i]))
						{
							return false;
						}
					}
				}
			}
		}
	}

	return true;
}

string json::request::json::FQDN()
{
	string result = identifier();

	json *current = _parent;
	while (current != NULL)	
	{	
		if(current->identifier().count() > 0L) result = current->identifier() + "\\" + result;
		current = current->_parent;
	};
	
	return result;
}

string json::response::json::extract()
{
	std::vector<core::custom::pair> result;

	for (unsigned long i = 0UL; i < pairs(); ++i)
	{
		core::custom::pair pair = pull(i);
		if (!pair.value.empty())
		{
			result.push_back(pair);
		}
	}

	if (result.size() == 0L) return string("");

	string data = "{\r\n";

	for (unsigned long i = 0UL; i < result.size() - 1UL; ++i)
	{		
		core::custom::pair pair = result[i];
		data += "\"" + pair.name + "\" : \"" + pair.value + "\",\r\n";
	}

	core::custom::pair pair = result[result.size() - 1L];
	data += "\"" + pair.name + "\" : \"" + pair.value + "\"\r\n";

	if (_child != NULL)
	{
		data += ",\r\n" + _child->extract();
	}

	data += "}\r\n";

	return data;
}