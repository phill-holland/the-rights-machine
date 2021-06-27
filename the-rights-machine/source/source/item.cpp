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

string data::item::item::output()
{
	string result("\"item\" : {\"itemID\" : ");
	result.concat(string::fromInt(itemID));
	result.concat(string(", \"messageID\" : "));
	result.concat(string::fromInt(messageID));
	result.concat(string(", \"name\" : \""));
	result.concat(name);
	result.concat(string("\"}\r\n"));

	return result;
	//Log << result;
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