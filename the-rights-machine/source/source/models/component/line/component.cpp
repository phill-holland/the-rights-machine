#include "models/component/line/component.h"

void models::component::line::component::clear()
{
	componentID = 0;
	lineID = 0;
	name = string("");
}

bool models::component::line::component::add(core::custom::pair source)
{
	if (string("name").icompare(source.name))
	{
		name = source.value;
		return true;
	}

	return false;
}

void models::component::line::component::copy(component const &source)
{
	componentID = source.componentID;
	lineID = source.lineID;
	name = source.name;
}

string models::component::line::component::output()
{
	string result("\"component\" : {\"componentID\" : ");
	result.concat(string::fromInt(componentID));
	result.concat(string(", \"lineID\" : "));
	result.concat(string::fromInt(lineID));
	result.concat(string(", \"name\" : \""));
	result.concat(name);
	result.concat(string("\"}\r\n"));

	return result;
}