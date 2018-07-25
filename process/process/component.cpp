#include "component.h"
#include "log.h"

void data::component::component::clear()
{
	componentID = 0;
	lineID = 0;
	name = "";
}

bool data::component::component::add(custom::pair &source)
{
	if (string("name").icompare(source.name))
	{
		name = source.value;
		return true;
	}

	return false;
}

void data::component::component::copy(component const &source)
{
	componentID = source.componentID;
	lineID = source.lineID;
	name = source.name;
}

void data::component::component::output()
{
	string result = "\"component\" : {\"componentID\" : ";
	result += string::fromInt(componentID);
	result += ", \"lineID\" : ";
	result += string::fromInt(lineID);
	result += ", \"name\" : \"";
	result += name;
	result += "\"}\r\n";

	Log << result;
}