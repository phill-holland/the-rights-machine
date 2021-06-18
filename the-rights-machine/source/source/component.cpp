#include "component.h"
#include "log.h"

void data::component::line::component::clear()
{
	componentID = 0;
	lineID = 0;
	name = string("");
}

bool data::component::line::component::add(custom::pair source)
{
	if (string("name").icompare(source.name))
	{
		name = source.value;
		return true;
	}

	return false;
}

void data::component::line::component::copy(component const &source)
{
	componentID = source.componentID;
	lineID = source.lineID;
	name = source.name;
}

void data::component::line::component::output()
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

void data::component::query::component::clear()
{
	componentID = 0;
	queryID = 0;
	name = string("");
}

bool data::component::query::component::add(custom::pair source)
{
	if (string("name").icompare(source.name))
	{
		name = source.value;
		return true;
	}

	return false;
}

void data::component::query::component::copy(component const &source)
{
	componentID = source.componentID;
	queryID = source.queryID;
	name = source.name;
}

void data::component::query::component::output()
{
	string result = "\"component\" : {\"componentID\" : ";
	result += string::fromInt(componentID);
	result += ", \"queryID\" : ";
	result += string::fromInt(queryID);
	result += ", \"name\" : \"";
	result += name;
	result += "\"}\r\n";

	Log << result;
}