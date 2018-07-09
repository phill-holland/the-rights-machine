#include "line.h"

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

/*
void data::line::line::reset()
{
	init = false; cleanup();

	components = new data::components::components<MAX>();
	if (components == NULL) return;
	if (!components->initalised()) return;

	clear();

	init = true;
}

void data::line::line::clear()
{
	base::clear();
	components->empty();
}

void data::line::line::copy(line const &source)
{
	base::copy(source);
	components->copy(*source.components);
}

void data::line::line::makeNull()
{
	components = NULL;
}

void data::line::line::cleanup()
{
	if (components != NULL) delete components;
}
*/