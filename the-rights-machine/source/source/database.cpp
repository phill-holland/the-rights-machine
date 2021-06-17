#include "database.h"

DWORD WINAPI queues::database::incoming::queue::background(thread *bt)
{
	if (counter > interval)
	{
		flush();
		poll();

		counter = 0UL;
	}

	++counter;

	Sleep(1000);

	return (DWORD)0;
}

void queues::database::incoming::queue::reset(::database::settings &settings, unsigned long interval)
{
	init = false; cleanup();

	counter = 0UL;

	this->settings = &settings;
	this->interval = interval;

	incoming = new custom::fifo<compute::task, LENGTH>();
	if (incoming == NULL) return;
	if (!incoming->initalised()) return;

	outgoing = new custom::fifo<compute::task, LENGTH>();
	if (outgoing == NULL) return;
	if (!outgoing->initalised()) return;

	message = new ::database::storage::message();
	if (message == NULL) return;

	init = true;
}

bool queues::database::incoming::queue::set(compute::task &source)
{
	if (incoming->entries() >= INPUT) flush();

	return incoming->set(source);
}

bool queues::database::incoming::queue::get(compute::task &destination)
{
	poll();

	if (outgoing->entries() > 0UL)
	{
		return outgoing->get(destination);
	}

	return false;
}

bool queues::database::incoming::queue::flush()
{
	mutex lock(flushing);

	unsigned long successful = 0UL, count = incoming->entries();

	if (count > 0UL)
	{
		if (message->open(*settings))
		{
			compute::task temp;

			for (unsigned long i = 0UL; i < incoming->entries(); ++i)
			{				
				if (incoming->get(temp))
				{
					if (message->write(temp.message)) ++successful;
				}				
			}

			message->close();
		} 
	}

	return successful == count;
}

bool queues::database::incoming::queue::poll()
{
	mutex lock(polling);

	if (outgoing->entries() == 0UL)
	{
		if (message->open(*settings))
		{
			compute::task temp;
			while ((message->read(temp.message)) && (!outgoing->isfull()))
			{
				if (!outgoing->set(temp)) return false;
			}
		}
	}	

	return true;
}

void queues::database::incoming::queue::makeNull()
{
	incoming = NULL;
	outgoing = NULL;
	message = NULL;
}

void queues::database::incoming::queue::cleanup()
{
	if (message != NULL) delete message;
	if (outgoing != NULL) delete outgoing;
	if (incoming != NULL) delete incoming;
}

void queues::database::incoming::factory::reset(::database::settings &settings, unsigned long total)
{
	init = false; cleanup();

	this->settings = &settings;
	this->total = total;
	length = 0UL;

	queues = new ::queue::queue<compute::task>*[total];
	if (queues == NULL) return;
	for (unsigned long i = 0UL; i < total; ++i) queues[i] = NULL;

	init = true;
}

::queue::queue<compute::task> *queues::database::incoming::factory::get()
{
	if (length >= MAX) return NULL;

	queue *result = new queues::database::incoming::queue(*settings);
	if (result != NULL)
	{
		queues[length++] = result;
	}

	return result;
}

void queues::database::incoming::factory::makeNull()
{
	queues = NULL;
}

void queues::database::incoming::factory::cleanup()
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

DWORD WINAPI queues::database::outgoing::queue::background(thread *bt)
{
	if (counter > interval)
	{
		flush();
		poll();

		counter = 0UL;
	}

	++counter;

	Sleep(1000);

	return (DWORD)0;
}

void queues::database::outgoing::queue::reset(::database::settings &settings, unsigned long interval)
{
	init = false; cleanup();

	counter = 0UL;

	this->settings = &settings;
	this->interval = interval;

	incoming = new custom::fifo<data::response::response, LENGTH>();
	if (incoming == NULL) return;
	if (!incoming->initalised()) return;

	outgoing = new custom::fifo<data::response::response, LENGTH>();
	if (outgoing == NULL) return;
	if (!outgoing->initalised()) return;

	response = new ::database::storage::response();
	if (response == NULL) return;

	init = true;
}

bool queues::database::outgoing::queue::set(data::response::response &source)
{
	if (incoming->entries() >= INPUT) flush();

	return incoming->set(source);
}

bool queues::database::outgoing::queue::get(data::response::response &destination)
{
	poll();

	if (outgoing->entries() > 0UL)
	{
		return outgoing->get(destination);
	}

	return false;
}

bool queues::database::outgoing::queue::flush()
{
	mutex lock(flushing);

	unsigned long successful = 0UL, count = incoming->entries();

	if (count > 0UL)
	{
		if (response->open(*settings))
		{
			data::response::response temp;

			for (unsigned long i = 0UL; i < incoming->entries(); ++i)
			{
				if (incoming->get(temp))
				{
					if (response->write(temp)) ++successful;
				}
			}

			response->close();
		}
	}

	return successful == count;
}

bool queues::database::outgoing::queue::poll()
{
	mutex lock(polling);

	if (outgoing->entries() == 0UL)
	{
		if (response->open(*settings))
		{
			data::response::response temp;
			while ((response->read(temp)) && (!outgoing->isfull()))
			{
				if (!outgoing->set(temp)) return false;
			}
		}
	}

	return true;
}

void queues::database::outgoing::queue::makeNull()
{
	incoming = NULL;
	outgoing = NULL;
	response = NULL;
}

void queues::database::outgoing::queue::cleanup()
{
	if (response != NULL) delete response;
	if (outgoing != NULL) delete outgoing;
	if (incoming != NULL) delete incoming;
}

void queues::database::outgoing::factory::reset(::database::settings &settings, unsigned long total)
{
	init = false; cleanup();

	this->total = total;
	length = 0UL;

	queues = new data::response::responses*[total];
	if (queues == NULL) return;
	for (unsigned long i = 0UL; i < total; ++i) queues[i] = NULL;

	init = true;
}

::custom::chain<data::response::response> *queues::database::outgoing::factory::get()
{
	if (length >= MAX) return NULL;

	data::response::responses *result = new data::response::responses();
	if (result != NULL)
	{
		queues[length++] = result;
	}

	return result;
}

void queues::database::outgoing::factory::makeNull()
{
	queues = NULL;
}

void queues::database::outgoing::factory::cleanup()
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
