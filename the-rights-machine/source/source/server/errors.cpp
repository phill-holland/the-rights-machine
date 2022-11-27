#include "server/errors.h"

void error::errors::background(thread *bt)
{
	sleep(200);
	++counter;
	
	if ((counter >= EXPIRATION) || (length >= THRESHOLD))
	{
		core::threading::mutex lock(token);

		flush();
	}
	else sleep(1000);
}

void error::errors::reset(::queue::in<::error::type::type> *destination)
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
	core::threading::mutex lock(token);

	reset();
}

bool error::errors::set(::error::error &source)
{
	core::threading::mutex lock(token);

	if (length >= MAX) return false;

	*data[length++] = source;

	return true;
}

bool error::errors::flush()
{
	if (destination != NULL)
	{
		for (long i = 0L; i < length; ++i)
		{
			::error::type::type temp = lookup(*data[i]);
			if (!destination->set(temp)) return false;
		}

		reset();
	}

	return true;
}

void error::errors::reset()
{
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