#include "buffer.h"
#include "log.h"

void buffer::reset(long len)
{
	cleanup();

	init = false; wrap = false;
	length = len;

	data = new char[length];
	if (data == NULL) return;

	clear();

	init = true;
}

void buffer::clear()
{
	lpwrite = 0L;
	memset(data, 0, length);
}

long buffer::indexOf(string &source)
{
	long result = indexOf(source.c_str(), source.count());

	return result;
}

long buffer::indexOf(const char *source, long src_length)
{
	long result = 0L;
	if (lpwrite<src_length) return false;

	long src = 0L, dest = 0L;

	while ((data[dest] != 0) && (dest < lpwrite))
	{
		if (data[dest] == source[src])
		{
			if (src == 0) result = dest;
			++src;
			if (src == src_length) return result;
		}
		else
		{
			src = 0L;
			result = 0L;
		}

		++dest;
	}

	return -1L;
}

bool buffer::put(char character)
{
	bool result = true;

	if (lpwrite >= length)
	{
		if (wrap) { result = false; lpwrite = 0L; }
		else return false;
	}

	data[lpwrite++] = character;

	return result;
}

bool buffer::put(string &source)
{
	return put(source.c_str(), source.count());
}

bool buffer::put(const char *character, long length)
{
	bool result = true;
	for (long i = 0L; i<length; ++i)
	{
		if (!put(character[i])) result = false;

	}
	return result;
}

bool buffer::get(long index, char &character)
{
	character = 0;
	if (index<lpwrite)
	{
		character = data[index];
		return true;
	}

	return false;
}

string buffer::get(long index, long length)
{
	string result;

	if ((index + length) <= lpwrite)
	{
		result.concat(&data[index], length);
	}

	return result;
}

bool buffer::copy(const buffer &source)
{
	bool result = true;

	clear();
	lpwrite = source.lpwrite;
	if (lpwrite >= length) { lpwrite = length; result = false; }

	memcpy(data, source.data, lpwrite);

	return result;
}

void buffer::makeNull()
{
	data = NULL;
}

void buffer::cleanup()
{
	if (data != NULL) delete data;
}