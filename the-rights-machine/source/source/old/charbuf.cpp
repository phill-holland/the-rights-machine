#include "charbuf.h"
//#include <cstring>

void global::charbuf::reset(unsigned long length)
{
	init = false; cleanup();

	this->length = length;
	buffer = new char[length];
	if (buffer == NULL) return;

	clear();

	init = true;
}

void global::charbuf::clear()
{
	items = 0UL;
	for(int i=0; i < length; ++i) buffer[i] = 0;
	//std::memset(buffer, 0, length);
}

bool global::charbuf::push(char value)
{
	if (items >= length) return false;
	buffer[items++] = value;

	return true;
}

bool global::charbuf::compare(string value)
{
	if (value.count() > (long)items) return false;

	for (unsigned long i = 0L; i < (unsigned long)value.count(); ++i)
	{
		if (buffer[i] != value[i]) return false;
	}

	return true;
}

void global::charbuf::copy(charbuf const &source)
{
	unsigned long len = source.items;
	if (source.items > length) len = length;

	clear();

	for (unsigned long i = 0UL; i < len; ++i)
	{
		push(source.buffer[i]);
	}
}

void global::charbuf::makeNull()
{
	buffer = NULL;
}

void global::charbuf::cleanup()
{
	if (buffer != NULL) delete buffer;
}