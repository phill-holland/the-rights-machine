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
/*
void data::item::item::reset()
{
	init = false; cleanup();

	lines = new data::lines::lines<MAX>();
	if (lines == NULL) return;
	if (!lines->initalised()) return;

	clear();

	init = true;
}

void data::item::item::clear()
{
	base::clear();
	lines->empty();
}

void data::item::item::copy(item const &source)
{
	base::copy(source);
	lines->copy(*source.lines);
}

void data::item::item::makeNull()
{
	lines = NULL;
}

void data::item::item::cleanup()
{
	if (lines != NULL) delete lines;
}
*/