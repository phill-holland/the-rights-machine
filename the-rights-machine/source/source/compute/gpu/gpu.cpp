#include "compute/gpu/gpu.h"
#include "core/string/string.h"
#include <unordered_map>
#include <unordered_set>
#include <vector>

void compute::gpu::gpu::background(thread *bt)
{
	sleep(100);

	::compute::task task;
	if (get(task))
	{
		process->push(task);
	}
	else
	{
		sleep(5000);
	}
}

void compute::gpu::gpu::reset(::queue::factory<::compute::task> *factory)
{
	init = false; cleanup();

	queue = factory->get();
	if (queue == NULL) return;

	process = new processor(255, 255);
	if (process == NULL) return;
	if (!process->initalised()) return;

	init = true;
}

void compute::gpu::gpu::makeNull()
{
	process = NULL;
}

void compute::gpu::gpu::cleanup()
{
	if (process != NULL) delete process;
}
