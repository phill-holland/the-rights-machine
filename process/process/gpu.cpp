#include "gpu.h"

DWORD WINAPI compute::gpu::background(thread *bt)
{
	return (DWORD)0;
}

void compute::gpu::reset(::queue::factory<::compute::task> *factory)
{
	init = false; cleanup();

	queue = factory->get();
	if (queue == NULL) return;

	init = true;
}

void compute::gpu::makeNull()
{
}

void compute::gpu::cleanup()
{
}
