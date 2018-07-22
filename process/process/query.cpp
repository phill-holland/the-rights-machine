#include "query.h"

void data::query::query::reset()
{
	init = false; cleanup();

	clear();

	components.json::parent(this);
	elements.json::parent(&components);

	init = true;
}

void data::query::query::clear()
{
	queryID = 0;
	messageID = 0;
	
	start.clear();
	end.clear();

	components.clear();
	elements.clear();
}

bool data::query::query::add(custom::pair &source)
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

	return false;
}

void data::query::query::copy(query const &source)
{
	queryID = source.queryID;
	messageID = source.messageID;
	start = source.start;
	end = source.end;

	components = source.components;
	elements = source.elements;
}


void data::query::query::makeNull()
{
}

void data::query::query::cleanup()
{
}