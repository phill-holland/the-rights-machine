#include "grid.h"
#include "log.h"

void compute::grid::reset(unsigned long width, unsigned long height)
{
	init = false; cleanup();

	this->width = width;
	this->height = height;

	write_ptr = 0UL;

	headers = new header*[height];
	if (headers == NULL) return;
	for (unsigned long i = 0UL; i < height; ++i) headers[i] = NULL;

	for (unsigned long i = 0UL; i < height; ++i)
	{
		headers[i] = new header();
		if (headers[i] == NULL) return;			
	}
	
	data = new int[width * height];
	if (data == NULL) return;

	clear();

	init = true;
}

void compute::grid::clear()
{
	write_ptr = 0UL;
	memset(data, 0, sizeof(int) * width * height);
	for (unsigned long i = 0UL; i < height; ++i) headers[i]->clear();
}

bool compute::grid::isempty()
{
	for (unsigned long i = 0UL; i < height; ++i)
	{
		if (!headers[i]->isempty()) return false;
	}

	return true;
}

void compute::grid::minus(grid &right)
{
	unsigned long offset = 0UL;

	for (unsigned long y = 0UL; y < height; ++y)
	{
		for (unsigned long x = 0UL; x < width; ++x)
		{
			if(data[offset + x] > 0) data[offset + x] -= right.data[offset + x];
		}

		offset += width;
	}
}

void compute::grid::and(grid &right)
{
	unsigned long offset = 0UL;

	for (unsigned long y = 0UL; y < height; ++y)
	{
		for (unsigned long x = 0UL; x < width; ++x)
		{
			data[offset + x] = data[offset + x] & right.data[offset + x];
		}

		offset += width;
	}
}

bool compute::grid::compare(grid &right)
{
	unsigned long offset = 0UL;

	for (unsigned long y = 0UL; y < height; ++y)
	{
		for (unsigned long x = 0UL; x < width; ++x)
		{
			if ((right.data[offset + x] == 1) && (data[offset + x] == 0)) return false;
		}

		offset += width;
	}

	return true;
}

bool compute::grid::push(row &source)
{
	if (source.length > width) return false;
	if (write_ptr >= height) return false;

	unsigned long offset = (write_ptr * width);

	*headers[write_ptr] = source.top;

	for (unsigned long i = 0UL; i < source.length; ++i)
	{
		data[offset + i] = source.data[i];
	}

	++write_ptr;

	return true;
}

void compute::grid::output()
{
	for (unsigned long i = 0UL; i < height; ++i)
	{
		if (!headers[i]->isempty())
		{
			string result = headers[i]->serialize();
			
			result += ",\"row\":{";
			if (data[0] > 0) result += "\"0\":1";

			for (unsigned long j = 1UL; j < width; ++j)
			{
				if (data[j] > 0) result += ",\"" + string::fromInt((int)j) + "\":1";
			}
			
			result += "}";

			Log << result << "\r\n";
		}
	}
}

/*
void compute::grid::extract(queue::in<result> *destination)
{
}
*/

void compute::grid::makeNull()
{
	headers = NULL;
	data = NULL;
}

void compute::grid::cleanup()
{
	if (headers != NULL)
	{
		for (long i = (height - 1L); i >= 0L; i--)
		{
			if (headers[i] != NULL) delete headers[i];
		}

		delete headers;
	}

	if (data != NULL) delete data;
}
