#include "queues/database.h"

void queues::database::incoming::queue::background(thread *bt)
{
	if (counter > interval)
	{
		flush();
		poll();

		counter = 0UL;
	}

	++counter;

	sleep(1000);
}

void queues::database::incoming::queue::reset(::database::settings &settings, unsigned long interval)
{
	init = false; cleanup();

	counter = 0UL;

	this->settings = &settings;
	this->interval = interval;

	incoming = new core::queue::fifo<compute::task, LENGTH>();
	if (incoming == nullptr) return;
	if (!incoming->initalised()) return;

	outgoing = new core::queue::fifo<compute::task, LENGTH>();
	if (outgoing == nullptr) return;
	if (!outgoing->initalised()) return;

	message = new ::database::storage::message();
	if (message == nullptr) return;

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
	core::threading::mutex lock(flushing);

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
	core::threading::mutex lock(polling);

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
	incoming = nullptr;
	outgoing = nullptr;
	message = nullptr;
}

void queues::database::incoming::queue::cleanup()
{
	if (message != nullptr) delete message;
	if (outgoing != nullptr) delete outgoing;
	if (incoming != nullptr) delete incoming;
}

void queues::database::incoming::factory::reset(::database::settings &settings, unsigned long total)
{
	init = false; cleanup();

	this->settings = &settings;
	this->total = total;
	length = 0UL;

	queues = new ::queue::queue<compute::task>*[total];
	if (queues == nullptr) return;
	for (unsigned long i = 0UL; i < total; ++i) queues[i] = nullptr;

	init = true;
}

::queue::queue<compute::task> *queues::database::incoming::factory::get()
{
	if (length >= MAX) return nullptr;

	queue *result = new queues::database::incoming::queue(*settings);
	if (result != nullptr)
	{
		queues[length++] = result;
	}

	return result;
}

void queues::database::incoming::factory::makeNull()
{
	queues = nullptr;
}

void queues::database::incoming::factory::cleanup()
{
	if (queues != nullptr)
	{
		for (long i = (total - 1L); i >= 0L; i--)
		{
			if (queues[i] != nullptr) delete queues[i];
		}

		delete queues;
	}
}

void queues::database::outgoing::queue::background(thread *bt)
{
	if (counter > interval)
	{
		flush();
		poll();

		counter = 0UL;
	}

	++counter;

	sleep(1000);

	//return (DWORD)0;
}

void queues::database::outgoing::queue::reset(::database::settings &settings, unsigned long interval)
{
	init = false; cleanup();

	counter = 0UL;

	this->settings = &settings;
	this->interval = interval;

	incoming = new core::queue::fifo<models::response::response, LENGTH>();
	if (incoming == nullptr) return;
	if (!incoming->initalised()) return;

	outgoing = new core::queue::fifo<models::response::response, LENGTH>();
	if (outgoing == nullptr) return;
	if (!outgoing->initalised()) return;

	response = new ::database::storage::response();
	if (response == nullptr) return;

	init = true;
}

bool queues::database::outgoing::queue::set(models::response::response &source)
{
	if (incoming->entries() >= INPUT) flush();

	return incoming->set(source);
}

bool queues::database::outgoing::queue::get(models::response::response &destination)
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
	core::threading::mutex lock(flushing);

	unsigned long successful = 0UL, count = incoming->entries();

	if (count > 0UL)
	{
		if (response->open(*settings))
		{
			models::response::response temp;

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
	core::threading::mutex lock(polling);

	if (outgoing->entries() == 0UL)
	{
		if (response->open(*settings))
		{
			models::response::response temp;
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
	incoming = nullptr;
	outgoing = nullptr;
	response = nullptr;
}

void queues::database::outgoing::queue::cleanup()
{
	if (response != nullptr) delete response;
	if (outgoing != nullptr) delete outgoing;
	if (incoming != nullptr) delete incoming;
}

void queues::database::outgoing::factory::reset(::database::settings &settings, unsigned long total)
{
	init = false; cleanup();

	this->total = total;
	length = 0UL;

	queues = new models::response::responses*[total];
	if (queues == nullptr) return;
	for (unsigned long i = 0UL; i < total; ++i) queues[i] = nullptr;

	init = true;
}

::custom::chain<models::response::response> *queues::database::outgoing::factory::get()
{
	if (length >= MAX) return nullptr;

	models::response::responses *result = new models::response::responses();
	if (result != nullptr)
	{
		queues[length++] = result;
	}

	return result;
}

void queues::database::outgoing::factory::makeNull()
{
	queues = nullptr;
}

void queues::database::outgoing::factory::cleanup()
{
	if (queues != nullptr)
	{
		for (long i = (total - 1L); i >= 0L; i--)
		{
			if (queues[i] != nullptr)
			{
				delete queues[i];
			}
		}

		delete queues;
	}
}
