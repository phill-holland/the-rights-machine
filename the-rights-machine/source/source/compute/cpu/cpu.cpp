#include "compute/cpu/cpu.h"
#include "compute/grid.h"
#include "compute/row.h"
#include "core/string/string.h"
#include <unordered_map>
#include <unordered_set>
#include <vector>

void compute::cpu::cpu::background(thread *bt)
{
	::compute::task task;
	if (get(task))
	{
		process->push(task);
	}
	else
	{
		sleep(250);
	}

	sleep(100);
}

void compute::cpu::cpu::reset(::queue::factory<::compute::task> *factory)
{
	init = false; cleanup();

	queue = factory->get();
	if (queue == NULL) return;

	process = new processor(255, 255);
	if (process == NULL) return;
	if (!process->initalised()) return;

	init = true;
}

void compute::cpu::cpu::makeNull()
{
	process = NULL;
}

void compute::cpu::cpu::cleanup()
{
	if (process != NULL) delete process;
}
