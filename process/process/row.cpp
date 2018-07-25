#include "row.h"
#include <windows.h>

void compute::row::reset(unsigned long length)
{
	init = false; cleanup();

	this->length = length;

	data = new int[length];
	if (data == NULL) return;

	clear();

	init = true;
}

void compute::row::clear()
{
	memset(data, 0, sizeof(int) * length);
	top.clear();
}

void compute::row::set(header &source)
{
	top = source;
}

bool compute::row::set(unsigned long idx)
{
	if (idx >= length) return false;

	data[idx] = 1;

	return true;
}

bool compute::row::copy(row const &source)
{
	bool result = true;
	if (length != source.length) result = false;

	clear();

	unsigned long temp = source.length;
	if (temp >= length) temp = length;

	for (unsigned long i = 0UL; i < temp; ++i)
	{
		data[i] = source.data[i];
	}

	top = source.top;

	return result;
}

void compute::row::makeNull()
{
	data = NULL;
}

void compute::row::cleanup()
{
	if (data != NULL) delete data;
}