#include "item.h"
#include "log.h"

void data::item::item::clear()
{
	itemID = 0;
	messageID = 0;
	name = string("");
}

void data::item::item::copy(item const &source)
{
	itemID = source.itemID;
	messageID = source.messageID;
	name = source.name;
}

void data::item::item::output()
{
	string result = "\"item\" : {\"itemID\" : ";
	result += string::fromInt(itemID);
	result += ", \"messageID\" : ";
	result += string::fromInt(messageID);
	result += ", \"name\" : \"";
	result += name;
	result += "\"}\r\n";

	Log << result;
}

bool data::item::item::add(custom::pair source)
{
	if (string("name").icompare(source.name))
	{
		name = source.value;
		return true;
	}

	return false;
}