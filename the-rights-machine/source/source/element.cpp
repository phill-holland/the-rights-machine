#include "element.h"
#include "log.h"

void data::element::element::clear()
{
	elementID = 0;
	componentID = 0;
	value = string("");
}

bool data::element::element::add(custom::pair source)
{
	if (string("value").icompare(source.name))
	{
		value = source.value;
		return true;
	}

	return false;
}

void data::element::element::copy(element const &source)
{
	elementID = source.elementID;
	componentID = source.componentID;
	value = source.value;
}

string data::element::element::output()
{
	string result("\"element\" : {\"elementID\" : ");
	result.concat(string::fromInt(elementID));
	result.concat(string(", \"componentID\" : "));
	result.concat(string::fromInt(componentID));
	result.concat(string(", \"value\" : \""));
	result.concat(value);
	result.concat(string("\"}\r\n"));

	return result;
}
