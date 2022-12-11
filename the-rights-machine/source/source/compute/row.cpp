#include "compute/row.h"

void compute::cpu::row::reset(unsigned long length)
{
	init = false; cleanup();

	this->length = length;

	data = new int[length];
	if (data == NULL) return;

	clear();

	init = true;
}

void compute::cpu::row::clear()
{
	for (unsigned long i = 0UL; i < length; ++i) data[i] = 0;
	
	top.clear();
}

int compute::cpu::row::get(unsigned long idx)
{
	if (idx >= length) return -1;

	return data[idx];
}

void compute::cpu::row::set(header *source)
{
	top = *source;
}

bool compute::cpu::row::set(unsigned long idx)
{
	if (idx >= length) return false;

	data[idx] = 1;

	return true;
}

bool compute::cpu::row::copy(row const &source)
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

void compute::cpu::row::makeNull()
{
	data = NULL;
}

void compute::cpu::row::cleanup()
{
	if (data != NULL) delete data;
}