#include "errors.h"
#include "log.h"

DWORD WINAPI error::errors::background(thread *bt)
{
	//Sleep(10);
	//return (DWORD)0;

	Sleep(200);
	++counter;
	
	mutex lock(token);

	if ((counter >= EXPIRATION) || (length >= THRESHOLD))
	{
		flush();
		//counter = 0L;
	}
	else Sleep(1000);

	return (DWORD)0;
}

void error::errors::reset(::queue::in<::error::type::type> *destination)//::queue::in<::error::error> *destination)
{
	init = false; cleanup();

	this->destination = destination;

	data = new ::error::error*[MAX];
	if (data == NULL) return;
	for (long i = 0L; i < MAX; ++i) data[i] = NULL;
	
	for (long i = 0L; i < MAX; ++i)
	{
		data[i] = new ::error::error();
		if (data[i] == NULL) return;
	}

	clear();

	init = true;
}

void error::errors::clear()
{
	mutex lock(token);

	reset();
}

bool error::errors::set(::error::error &source)
{
	mutex lock(token);

	if (length >= MAX) return false;
	//queue.push_back(source);

	*data[length++] = source;

	return true;
}

bool error::errors::flush()
{
	if (destination != NULL) //&& (data != NULL))
	{
		//mutex lock(token);

		for (long i = 0L; i < length; ++i)// (long)queue.size(); ++i)
		{
			//if (!destination->set(queue[i])) return false;
			if (!destination->set(lookup(*data[i]))) return false;
		}

		reset();
	}

	return true;
}

void error::errors::reset()
{
	//queue.clear();
	length = 0L;
	counter = 0L;
}

void error::errors::makeNull()
{
	destination = NULL;
	data = NULL;
}

void error::errors::cleanup()
{
	if (data != NULL)
	{
		for (long i = (MAX - 1L); i >= 0L; i--)
		{
			if (data[i] != NULL) delete data[i];
		}
		delete data;
	}
}