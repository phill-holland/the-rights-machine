#include "json.h"
#include "log.h"
#include <vector>

string data::json::request::json::FQDN()
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

string data::json::response::json::extract()
{
	std::vector<custom::pair> result;

	for (unsigned long i = 0UL; i < pairs(); ++i)
	{
		custom::pair pair = pull(i);
		if(!pair.value.empty()) result.push_back(pair);
	}

	if (result.size() == 0L) return string("");

	string data = "\"" + identifier() + "\" : \r\n{\r\n";

	for (unsigned long i = 0UL; i < result.size() - 1UL; ++i)
	{		
		custom::pair pair = result[i];
		data += "\"" + pair.name + "\" : \"" + pair.value + "\",\r\n";
	}

	custom::pair pair = result[result.size() - 1L];
	data += "\"" + pair.name + "\" : \"" + pair.value + "\"\r\n";

	if (_child != NULL)
	{
		data += ",\r\n" + _child->extract();
	}

	data += "}\r\n";

	return data;
}