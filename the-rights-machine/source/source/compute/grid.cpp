#include "compute/grid.h"
#include <cstring>
#include <iostream>

void compute::cpu::grid::reset(unsigned long width, unsigned long height)
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

void compute::cpu::grid::clear()
{
	write_ptr = 0UL;
	memset(data, 0, sizeof(int) * width * height);
	for (unsigned long i = 0UL; i < height; ++i) headers[i]->clear();
}

bool compute::cpu::grid::isempty()
{
	for (unsigned long i = 0UL; i < height; ++i)
	{
		if (!headers[i]->isempty()) return false;
	}

	return true;
}

void compute::cpu::grid::minus(grid &right)
{
	unsigned long offset = 0UL;

	for (unsigned long y = 0UL; y < height; ++y)
	{
		if(!headers[y]->isempty())
		{
		if(headers[y]->overlapped(*right.headers[y]))
		{
			for (unsigned long x = 0UL; x < width; ++x)
			{
				if (data[offset + x] > 0) data[offset + x] -= right.data[offset + x];
			}
		}
		}
		offset += width;
	}
}

void compute::cpu::grid::AND(grid &right)
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

bool compute::cpu::grid::compare(grid &right)
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

bool compute::cpu::grid::push(::compute::interfaces::row *source)
{
	if (source->count() > width) return false;
	if (write_ptr >= height) return false;

	unsigned long offset = (write_ptr * width);

	if(!source->first(headers[write_ptr])) return false;

	for (unsigned long i = 0UL; i < source->count(); ++i)
	{
		data[offset + i] = source->get(i);
	}

	++write_ptr;

	return true;
}

string compute::cpu::grid::output()
{
	string result;

	for (unsigned long i = 0UL; i < height; ++i)
	{
		if (!headers[i]->isempty())
		{
			result.concat(headers[i]->serialize());

			result.concat(string(",\"row\":{"));
			if (data[0] > 0) result.concat(string("\"0\":1"));

			for (unsigned long j = 1UL; j < width; ++j)
			{
				if (data[j] > 0) 
				{
					result.concat(string(",\""));
					result.concat(string::fromInt((int)j));
					result.concat(string("\":1"));
				}
			}

			result.concat(string("}\r\n"));
		}
	}

	return result;
}

void compute::cpu::grid::makeNull()
{
	headers = NULL;
	data = NULL;
}

void compute::cpu::grid::cleanup()
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
