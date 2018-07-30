#include "queues.h"

void queues::memory::incoming::factory::reset()
{
	init = false; cleanup();

	init = true;
}

::queue::queue<compute::task> *queues::memory::incoming::factory::get()
{
	queue *result = new queues::memory::incoming::queue();
	if (result != NULL) queues.push_back(result);
	return result;
}

void queues::memory::incoming::factory::makeNull()
{

}

void queues::memory::incoming::factory::cleanup()
{
	for (long i = 0L; i < queues.size(); ++i)
	{
		delete queues[i];
	}
}

void queues::memory::outgoing::factory::reset()
{
	init = false; cleanup();

	init = true;
}

::custom::chain<data::response::response> *queues::memory::outgoing::factory::get()
{
	data::response::responses *result = new data::response::responses();
	if (result != NULL) queues.push_back(result);
//	queue *result = new queues::memory::outgoing::queue();
	//if (result != NULL) queues.push_back(result);
	return result;
}
/*
::queue::queue<data::response::response> *queues::memory::outgoing::factory::get()
{
	queue *result = new queues::memory::outgoing::queue();
	if (result != NULL) queues.push_back(result);
	return result;
}
*/
void queues::memory::outgoing::factory::makeNull()
{

}

void queues::memory::outgoing::factory::cleanup()
{
	for (long i = 0L; i < queues.size(); ++i)
	{
		delete queues[i];
	}
}

void queues::database::incoming::queue::reset(::database::connection *connection)
{
	init = false; cleanup();

	incoming = new custom::fifo<data::message::message, LENGTH>();
	if (incoming == NULL) return;
	if (!incoming->initalised()) return;

	outgoing = new custom::fifo<data::message::message, LENGTH>();
	if (outgoing == NULL) return;
	if (!outgoing->initalised()) return;

	init = true;
}

bool queues::database::incoming::queue::set(data::message::message &source)
{
	if (incoming->entries() >= INPUT)
	{
		// push to database
	}

	return incoming->set(source);
	// shove into incoming queue
	// if queue exceeds X amount
	// flush to database
}

bool queues::database::incoming::queue::get(data::message::message &destination)
{
	if (outgoing->entries() == 0L) 
	{
		// also need some sort of timeout, don't want to keep
		// hitting the database

		// pull from database
	}

	return outgoing->get(destination);
	// if nothing in buffer, load 100L items from database into buffer
	// reutrn buffer
}

bool queues::database::incoming::queue::flush()
{
	// flush "put" queue to database

	if (incoming->entries() > 0UL)
	{
		// push to database
	}

	return true;
}

void queues::database::incoming::queue::makeNull()
{
	incoming = NULL;
	outgoing = NULL;
}

void queues::database::incoming::queue::cleanup()
{
	if (outgoing != NULL) delete outgoing;
	if (incoming != NULL) delete incoming;
}