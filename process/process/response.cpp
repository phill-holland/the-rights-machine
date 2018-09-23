#include "response.h"
#include "log.h"

void data::response::response::clear()
{
	guid = string("");
	user = string("");
	status = STATUS::OK;
	//queryID = 0;
	available = false;
	created = datetime::now();
}

unsigned long data::response::response::pairs()
{
	return 4UL;
}

custom::pair data::response::response::pull(unsigned long index)
{
	custom::pair result;

	if (index == 0UL) result = custom::pair(string("GUID"), guid);
	else if (index == 1UL) result = custom::pair(string("status"), map(status));
	else if (index == 2UL) result = custom::pair(string("available"), available == true ? string("true") : string("false"));
	else if (index == 3UL) result = custom::pair(string("created"), (string)created);

	return result;
}

string data::response::response::map(STATUS source)
{
	if (source == STATUS::OK) return string("OK");
	else if (source == STATUS::PENDING) return string("PENDING");
	
	return string("UNKNOWN");
}