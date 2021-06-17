#include "line.h"
#include "log.h"
#include <tuple>

void data::line::line::clear()
{
	lineID = 0;
	itemID = 0;
	start.clear();
	end.clear();
	exclusivityID = 0;
	typeID = 0;
}

void data::line::line::copy(line const &source)
{
	lineID = source.lineID;
	itemID = source.itemID;
	start = source.start;
	end = source.end;
	exclusivityID = source.exclusivityID;
	typeID = source.typeID;
}

void data::line::line::output()
{
	string result = "\"line\" : {";
	result += "\"start\" : \"";
	result += (string)start;
	result += "\", \"end\" : \"";
	result += (string)end;
	result += "\", \"lineID\" : ";
	result += string::fromInt(lineID);
	result += ", \"itemID\" : ";
	result += string::fromInt(itemID);
	result += ", \"typeID\" : ";
	result += string::fromInt(typeID);
	result += ", \"exclusivityID\" : ";
	result += string::fromInt(exclusivityID);
	result += "}\r\n";

	Log << result;
}

data::line::line data::line::line::spawn(datetime &start, datetime &end)
{
	data::line::line result(*this);

	result.start = start;
	result.end = end;

	return result;
}

bool data::line::line::add(custom::pair &source)
{
	if (string("start").icompare(source.name))
	{
		start.from(source.value);
		return true;
	}

	if (string("end").icompare(source.name))
	{
		end.from(source.value);
		return true;
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