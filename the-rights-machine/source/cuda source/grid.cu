#include "grid.cuh"
#include <thrust/reduce.h>
#include <thrust/device_ptr.h>
#include <thrust/execution_policy.h>
#include "custom/string.h"
#include "log.h"

unsigned long compute::gpu::grid::GRIDS = 255;
unsigned long compute::gpu::grid::THREADS = 255;

unsigned long compute::gpu::grid::WIDTH = 255;
unsigned long compute::gpu::grid::HEIGHT = 255;

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

__global__ void compareKernel(const int *a, const int *b, int *result)
{
	int i = threadIdx.x;

	result[i] = ((!(b[i] & 0x1)) & (a[i] & 0x1));
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
	if (!cudaMalloc((void**)&temp, width * height * sizeof(int))) return;

	clear();

	init = true;
}

void compute::gpu::grid::clear()
{
	write_ptr = 0UL;
	cudaMemset(data, 0, width * height * sizeof(int));
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

void compute::gpu::grid::AND(grid &right)
{
	andKernel<<<GRIDS, THREADS>>>(data, right.data);
}

bool compute::gpu::grid::compare(grid &right)
{
	compareKernel<<<GRIDS, THREADS>>>(data, right.data, temp);

	thrust::device_ptr<int> t_ptr = thrust::device_pointer_cast(temp);
	int result = thrust::reduce(thrust::device, t_ptr, t_ptr + (width * height));

	return result == 0;	
}

bool compute::gpu::grid::push(compute::common::row *source)
{
	if (source->count() > width) return false;
	if (write_ptr >= height) return false;

	unsigned long offset = (write_ptr * width);

	*headers[write_ptr] = source->first();

	if (cudaMemcpy(&data[offset], source->raw(), source->count() * sizeof(int), cudaMemcpyKind::cudaMemcpyHostToDevice) != cudaSuccess) return false;

	++write_ptr;

	return true;
}

void compute::gpu::grid::output()
{
	int *temp = new int[width * height];
	if (temp == NULL) return;

	if (cudaMemcpy(temp, data, width * height * sizeof(int), cudaMemcpyKind::cudaMemcpyDeviceToHost) != cudaSuccess)
	{
		for (unsigned long i = 0UL; i < height; ++i)
		{
			if (!headers[i]->isempty())
			{
				string result = headers[i]->serialize();

				result += ",\"row\":{";
				if (temp[0] > 0) result += "\"0\":1";

				for (unsigned long j = 1UL; j < width; ++j)
				{
					if (temp[j] > 0) result += ",\"" + string::fromInt((int)j) + "\":1";
				}

				result += "}";

				Log << result << string("\r\n");
			}
		}
	}

	delete temp;
}

void compute::gpu::grid::makeNull()
{
	headers = NULL;
	data = NULL;
	temp = NULL;
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

	if (temp != NULL) cudaFree(temp);
	if (data != NULL) cudaFree(data);
}
