#include "grid.cuh"
#include "log.h"

__global__ void minusKernel(int *a, const int *b)
{
	int i = threadIdx.x;

	a[i] = a[i] - b[i];
}

__global__ void andKernel(int *a, const int *b)
{
	int i = threadIdx.x;

	a[i] = a[i] & b[i];
}

void compute::gpu::grid::reset(unsigned long width, unsigned long height)
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

	if (!cudaMalloc((void**)&data, width * height * sizeof(int))) return;

	clear();

	init = true;
}

void compute::gpu::grid::clear()
{
	write_ptr = 0UL;
	memset(data, 0, sizeof(int) * width * height);
	for (unsigned long i = 0UL; i < height; ++i) headers[i]->clear();
}

bool compute::gpu::grid::isempty()
{
	for (unsigned long i = 0UL; i < height; ++i)
	{
		if (!headers[i]->isempty()) return false;
	}

	return true;
}

void compute::gpu::grid::minus(grid &right)
{
	minusKernel<<<GRIDS, THREADS>>>(data, right.data);
}

void compute::gpu::grid::and(grid &right)
{
	andKernel<<<GRIDS, THREADS>>>(data, right.data);
}

bool compute::gpu::grid::compare(grid &right)
{
	// reduce kernel here, need temp???
	//unsigned long offset = 0UL;

	/*
	for (unsigned long y = 0UL; y < height; ++y)
	{
		for (unsigned long x = 0UL; x < width; ++x)
		{
			if ((right.data[offset + x] == 1) && (data[offset + x] == 0)) return false;
		}

		offset += width;
	}
	*/
	return true;
}

bool compute::gpu::grid::push(row &source)
{
	if (source.length > width) return false;
	if (write_ptr >= height) return false;

	unsigned long offset = (write_ptr * width);

	*headers[write_ptr] = source.top;

	if (cudaMemcpy(&data[offset], source.data, source.length * sizeof(int), cudaMemcpyKind::cudaMemcpyHostToDevice) != cudaSuccess) return false;

	++write_ptr;

	return true;
}

void compute::gpu::grid::output()
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

void compute::gpu::grid::makeNull()
{
	headers = NULL;
	data = NULL;
}

void compute::gpu::grid::cleanup()
{
	if (headers != NULL)
	{
		for (long i = (height - 1L); i >= 0L; i--)
		{
			if (headers[i] != NULL) delete headers[i];
		}

		delete headers;
	}

	if (data != NULL) cudaFree(data);
}
