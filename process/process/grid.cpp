#include "grid.h"


void compute::grid::reset(unsigned long width, unsigned long height)
{
	init = false; cleanup();

	this->width = width;
	this->height = height;

	data = new int[width * height];
	if (data == NULL) return;

	clear();

	init = true;
}

void compute::grid::clear()
{
	memset(data, 0, sizeof(int) * width * height);
}

void compute::grid::minus(grid &right)
{
	unsigned long offset = 0UL;

	for (unsigned long y = 0UL; y < height; ++y)
	{
		for (unsigned long x = 0UL; x < width; ++x)
		{
			data[offset + x] -= right.data[offset + x];
		}

		offset += width;
	}
}

void compute::grid::push(row &source)
{
}

void compute::grid::makeNull()
{
	data = NULL;
}

void compute::grid::cleanup()
{
	if (data != NULL) delete data;
}
