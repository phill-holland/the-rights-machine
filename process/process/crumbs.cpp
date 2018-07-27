#include "crumbs.h"

void crumbs::crumbs::reset()
{
	init = false; cleanup();

	data = new string[DEPTH];
	if (data == NULL) return;

	init = true;
}

void crumbs::crumbs::clear()
{
	depth = 0L;
}

bool crumbs::crumbs::push(string value)
{
	if (depth >= DEPTH) return false;
	
	data[depth++] = value;

	return true;
}

bool crumbs::crumbs::pop()
{
	if (depth == 0UL) return false;
	
	--depth;

	return true;
}

string crumbs::crumbs::FQDN(string label)
{
	string result = label;

	long i = depth - 1L;
	while (i > 0L)
	{
		if (data[i].count() > 0L)
		{
			if (result.count() > 0) result = data[i] + "\\" + result;
			else result = data[i];
		}
		--i;
	};

	return result;
}

void crumbs::crumbs::makeNull()
{
	data = NULL;
}

void crumbs::crumbs::cleanup()
{
	if (data != NULL) delete data;
}