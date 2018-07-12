#include "cpu.h"

// has many worker threads for grid subtraction

DWORD WINAPI compute::cpu::background(thread *bt)
{
	return (DWORD)0;
}

void compute::cpu::reset(::queue::factory<::compute::task> *factory)
{
	init = false; cleanup();

	queue = factory->get();
	if (queue == NULL) return;

	init = true;
}

void compute::cpu::makeNull()
{
}

void compute::cpu::cleanup()
{
}
