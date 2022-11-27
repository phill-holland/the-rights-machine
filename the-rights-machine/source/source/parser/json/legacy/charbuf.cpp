#include "parser/json/legacy/charbuf.h"
#include <memory.h>

void parser::json::legacy::charbuf::reset(unsigned long length)
{
	init = false; cleanup();

	this->length = length;
	buffer = new char[length];
	if (buffer == NULL) return;

	clear();

	init = true;
}

void parser::json::legacy::charbuf::clear()
{
	items = 0UL;
	memset(buffer, 0, length);
}

bool parser::json::legacy::charbuf::push(char value)
{
	if (items >= length) return false;
	buffer[items++] = value;

	return true;
}

bool parser::json::legacy::charbuf::compare(string value)
{
	if (value.count() > (long)items) return false;

	for (unsigned long i = 0L; i < (unsigned long)value.count(); ++i)
	{
		if (buffer[i] != value[i]) return false;
	}

	return true;
}

void parser::json::legacy::charbuf::copy(charbuf const &source)
{
	unsigned long len = source.items;
	if (source.items > length) len = length;

	clear();

	for (unsigned long i = 0UL; i < len; ++i)
	{
		push(source.buffer[i]);
	}
}

void parser::json::legacy::charbuf::makeNull()
{
	buffer = NULL;
}

void parser::json::legacy::charbuf::cleanup()
{
	if (buffer != NULL) delete[] buffer;
}