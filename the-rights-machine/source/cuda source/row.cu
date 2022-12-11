#include "row.cuh"

void compute::gpu::row::reset(unsigned long length)
{
	init = false; cleanup();

	this->length = length;

	if (cudaMallocHost((void**)&data, length * sizeof(int)) != cudaSuccess) return;
	if (data == NULL) return;

	clear();

	init = true;
}

void compute::gpu::row::clear()
{
	for (unsigned long i = 0UL; i < length; ++i) data[i] = 0;

	top.clear();
}

int compute::gpu::row::get(unsigned long idx)
{
	if (idx >= length) return -1;

	return data[idx];
}

void compute::gpu::row::set(header *source)
{
	top = *source;
}

bool compute::gpu::row::set(unsigned long idx)
{
	if (idx >= length) return false;

	data[idx] = 1;

	return true;
}

bool compute::gpu::row::copy(row const &source)
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

void compute::gpu::row::makeNull()
{
	data = NULL;
}

void compute::gpu::row::cleanup()
{
	if (data != NULL) cudaFreeHost(data);
}