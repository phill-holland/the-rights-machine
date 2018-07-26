#include "errors.h"

DWORD WINAPI error::errors::background(thread *bt)
{
	Sleep(200);
	++counter;
	
	if ((counter >= EXPIRATION) || ((long)queue.size() >= THRESHOLD))
	{
		flush();
		counter = 0L;
	}
	else Sleep(1000);

	return (DWORD)0;
}

void error::errors::reset(::queue::in<::error::error> *destination)
{
	init = false; cleanup();

	this->destination = destination;

	clear();

	init = true;
}

void error::errors::clear()
{
	mutex lock(token);

	queue.clear();

	counter = 0L;
}

bool error::errors::set(::error::error &source)
{
	mutex lock(token);

	queue.push_back(source);

	return true;
}

bool error::errors::flush()
{
	if (destination != NULL)
	{
		mutex lock(token);

		for (long i = 0L; i < (long)queue.size(); ++i)
		{
			if (!destination->set(queue[i])) return false;
		}

		clear();
	}

	return true;
}

void error::errors::makeNull()
{
	destination = NULL;
}

void error::errors::cleanup()
{
}