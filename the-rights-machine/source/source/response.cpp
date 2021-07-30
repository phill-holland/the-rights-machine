#include "response.h"
#include "log.h"

void data::response::response::clear()
{
	guid.clear();
	name.clear();
	user.clear();
	status = STATUS::OK;
	available = false;
	created = datetime::now();
}

unsigned long data::response::response::pairs()
{
	return 5UL;
}

custom::pair data::response::response::pull(unsigned long index)
{
	custom::pair result;

	if (index == 0UL) result = custom::pair(string("GUID"), guid);
	else if (index == 1UL) result = custom::pair(string("status"), map(status));
	else if (index == 2UL) result = custom::pair(string("available"), available == true ? string("true") : string("false"));
	else if (index == 3UL) result = custom::pair(string("created"), (string)created);
	else if (index == 4UL) result = custom::pair(string("name"), name);

	return result;
}

bool data::response::response::validate(data::request::request &request)
{
	if ((guid.icompare(request.guid)) && (user.icompare(request.user)))
	{
		return true;
	}

	return false;
}

string data::response::response::map(STATUS source)
{
	if (source == STATUS::OK) return string("OK");
	else if (source == STATUS::PENDING) return string("PENDING");
	else if (source == STATUS::ERR) return string("ERR");
	else if (source == STATUS::RANGE) return string("RANGE");
	
	return string("UNKNOWN");
}