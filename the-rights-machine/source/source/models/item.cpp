#include "models/item.h"

void models::item::item::clear()
{
	itemID = 0;
	messageID = 0;
	name = string("");
}

void models::item::item::copy(item const &source)
{
	itemID = source.itemID;
	messageID = source.messageID;
	name = source.name;
}

string models::item::item::output()
{
	string result("\"item\" : {\"itemID\" : ");
	result.concat(string::fromInt(itemID));
	result.concat(string(", \"messageID\" : "));
	result.concat(string::fromInt(messageID));
	result.concat(string(", \"name\" : \""));
	result.concat(name);
	result.concat(string("\"}\r\n"));

	return result;
}

bool models::item::item::add(core::custom::pair source)
{
	if (string("name").icompare(source.name))
	{
		name = source.value;
		return true;
	}

	return false;
}