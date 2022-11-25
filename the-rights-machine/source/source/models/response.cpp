#include "models/response.h"
#include "core/string/string.h"

void models::response::response::clear()
{
	guid.clear();
	name.clear();
	user.clear();
	status = STATUS::OK;
	available = false;
	created = types::datetime::now();
}

unsigned long models::response::response::pairs()
{
	return 5UL;
}

core::custom::pair models::response::response::pull(unsigned long index)
{
	core::custom::pair result;

	if (index == 0UL) result = core::custom::pair(string("GUID"), guid);
	else if (index == 1UL) result = core::custom::pair(string("status"), map(status));
	else if (index == 2UL) result = core::custom::pair(string("available"), available == true ? string("true") : string("false"));
	else if (index == 3UL) result = core::custom::pair(string("created"), created.to());
	else if (index == 4UL) result = core::custom::pair(string("name"), name);

	return result;
}

bool models::response::response::validate(models::request::request &request)
{
	if ((guid.icompare(request.guid)) && (user.icompare(request.user)))
	{
		return true;
	}

	return false;
}

string models::response::response::map(STATUS source)
{
	if (source == STATUS::OK) return string("OK");
	else if (source == STATUS::PENDING) return string("PENDING");
	else if (source == STATUS::ERR) return string("ERR");
	else if (source == STATUS::RANGE) return string("RANGE");
	
	return string("UNKNOWN");
}