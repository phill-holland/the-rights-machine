#include "models/component/query/component.h"

void models::component::query::component::clear()
{
	componentID = 0;
	queryID = 0;
	name = string("");
}

bool models::component::query::component::add(core::custom::pair source)
{
	if (string("name").icompare(source.name))
	{
		name = source.value;
		return true;
	}

	return false;
}

void models::component::query::component::copy(component const &source)
{
	componentID = source.componentID;
	queryID = source.queryID;
	name = source.name;
}

string models::component::query::component::output()
{
	string result("\"component\" : {\"componentID\" : ");
	result.concat(string::fromInt(componentID));
	result.concat(string(", \"queryID\" : "));
	result.concat(string::fromInt(queryID));
	result.concat(string(", \"name\" : \""));
	result.concat(name);
	result.concat(string("\"}\r\n"));

	return result;
}