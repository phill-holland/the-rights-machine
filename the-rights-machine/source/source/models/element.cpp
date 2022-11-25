#include "models/element.h"

void models::element::element::clear()
{
	elementID = 0;
	componentID = 0;
	value = string("");
}

bool models::element::element::add(core::custom::pair source)
{
	if (string("value").icompare(source.name))
	{
		value = source.value;
		return true;
	}

	return false;
}

void models::element::element::copy(element const &source)
{
	elementID = source.elementID;
	componentID = source.componentID;
	value = source.value;
}

string models::element::element::output()
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
