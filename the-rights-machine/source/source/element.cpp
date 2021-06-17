#include "element.h"
#include "log.h"

void data::element::element::clear()
{
	elementID = 0;
	componentID = 0;
	value = string("");
}

bool data::element::element::add(custom::pair &source)
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

void data::element::element::output()
{
	string result = "\"element\" : {\"elementID\" : ";
	result += string::fromInt(elementID);
	result += ", \"componentID\" : ";
	result += string::fromInt(componentID);
	result += ", \"value\" : \"";
	result += value;
	result += "\"}\r\n";

	Log << result;
}
