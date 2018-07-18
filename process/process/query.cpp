#include "query.h"

void data::query::query::clear()
{
	queryID = 0;
	messageID = 0;
	start.clear();
	end.clear();
}

/*
data::query::query data::query::query::spawn(datetime &start, datetime &end)
{
	data::query::query result(*this);

	result.start = start;
	result.end = end;

	return result;
}
*/
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
}
