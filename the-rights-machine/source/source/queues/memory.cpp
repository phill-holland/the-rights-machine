#include "queues/memory.h"

void queues::memory::incoming::factory::reset(unsigned long total)
{
	init = false; cleanup();

	this->total = total;
	length = 0UL;

	queues = new ::queue::queue<compute::task>*[total];
	if (queues == NULL) return;
	for (unsigned long i = 0UL; i < total; ++i) queues[i] = NULL;

	init = true;
}

::queue::queue<compute::task> *queues::memory::incoming::factory::get()
{
	if (length >= MAX) return NULL;

	::queue::queue<compute::task> *result = new queues::memory::incoming::queue();
	if (result != NULL)
	{
		queues[length++] = result;
	}

	return result;
}

void queues::memory::incoming::factory::makeNull()
{
	queues = NULL;
}

void queues::memory::incoming::factory::cleanup()
{
	if (queues != NULL)
	{
		for (long i = (total - 1L); i >= 0L; i--)
		{
			if (queues[i] != NULL) delete queues[i];
		}

		delete queues;
	}
}

void queues::memory::outgoing::factory::reset(unsigned long total)
{
	init = false; cleanup();

	this->total = total;
	length = 0UL;

	queues = new models::response::responses*[total];
	if (queues == NULL) return;
	for (unsigned long i = 0UL; i < total; ++i) queues[i] = NULL;

	init = true;
}

::custom::chain<models::response::response> *queues::memory::outgoing::factory::get()
{
	if (length >= MAX) return NULL;

	models::response::responses *result = new models::response::responses();
	if (result != NULL)
	{
		queues[length++] = result;
	}

	return result;
}

void queues::memory::outgoing::factory::makeNull()
{
	queues = NULL;
}

void queues::memory::outgoing::factory::cleanup()
{
	if (queues != NULL)
	{
		for (long i = (total - 1L); i >= 0L; i--)
		{
			if (queues[i] != NULL)
			{
				delete queues[i];
			}
		}

		delete queues;
	}
}
