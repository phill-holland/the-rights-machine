#include "models/line.h"
#include <tuple>
#include <iostream>

void models::line::line::clear()
{
	lineID = 0;
	itemID = 0;
	start.clear();
	end.clear();
	exclusivityID = 0;
	typeID = 0;
}

void models::line::line::copy(line const &source)
{
	lineID = source.lineID;
	itemID = source.itemID;
	start = source.start;
	end = source.end;
	exclusivityID = source.exclusivityID;
	typeID = source.typeID;
}

string models::line::line::output()
{
	string result("\"line\" : {");
	result.concat(string("\"start\" : \""));
	result.concat((string)start);
	result.concat(string("\", \"end\" : \""));
	result.concat((string)end);
	result.concat(string("\", \"lineID\" : "));
	result.concat(string::fromInt(lineID));
	result.concat(string(", \"itemID\" : "));
	result.concat(string::fromInt(itemID));
	result.concat(string(", \"typeID\" : "));
	result.concat(string::fromInt(typeID));
	result.concat(string(", \"exclusivityID\" : "));
	result.concat(string::fromInt(exclusivityID));
	result.concat(string("}\r\n"));

	return result;
}

models::line::line models::line::line::spawn(datetime &start, datetime &end)
{
	models::line::line result(*this);

	result.start = start;
	result.end = end;

	return result;
}

bool models::line::line::add(core::custom::pair source)
{
	if (string("start").icompare(source.name))
	{
		return start.from(source.value);
	}

	if (string("end").icompare(source.name))
	{
		return end.from(source.value);
	}

	if (string("exclusivity").icompare(source.name))
	{
		exclusivityID = source.value.toInteger();
		return true;
	}

	if (string("type").icompare(source.name))
	{
		typeID = source.value.toInteger();
		return true;
	}

	return false;
}