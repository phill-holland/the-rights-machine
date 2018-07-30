#include "response.h"

unsigned long data::response::response::pairs()
{
	return 2UL;
}

custom::pair& data::response::response::pull(unsigned long index)
{
	custom::pair result;

	if (index == 0UL) result = custom::pair(string("GUID"), GUID);
//	else if (index == 1UL) result = custom::pair(string("status"), status);

	return result;
}