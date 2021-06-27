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

string data::component::line::component::output()
{
	string result("\"component\" : {\"componentID\" : ");
	result.concat(string::fromInt(componentID));
	result.concat(string(", \"lineID\" : "));
	result.concat(string::fromInt(lineID));
	result.concat(string(", \"name\" : \""));
	result.concat(name);
	result.concat(string("\"}\r\n"));

	return result;
	//Log << result;
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

string data::component::query::component::output()
{
	string result("\"component\" : {\"componentID\" : ");
	result.concat(string::fromInt(componentID));
	result.concat(string(", \"queryID\" : "));
	result.concat(string::fromInt(queryID));
	result.concat(string(", \"name\" : \""));
	result.concat(name);
	result.concat(string("\"}\r\n"));

	return result;
	//Log << result;
}