#include "item.h"

void data::item::item::clear()
{
	itemID = 0;
	messageID = 0;
	name = "";
}

void data::item::item::copy(item const &source)
{
	itemID = source.itemID;
	messageID = source.messageID;
	name = source.name;
}

bool data::item::item::add(custom::pair &source)
{
	if (string("name").icompare(source.name))
	{
		name = source.value;
		return true;
	}

	return false;
}