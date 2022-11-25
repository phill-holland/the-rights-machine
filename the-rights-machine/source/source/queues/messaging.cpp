#include "messaging.h"

void messaging::memory::memory::reset()
{
	init = false; cleanup();

	messages = new queues::memory::incoming::factory();
	if (messages == NULL) return;
	if (!messages->initalised()) return;

	responses = new queues::memory::outgoing::factory();
	if (responses == NULL) return;
	if (!responses->initalised()) return;

	init = true;
}

::queue::factory<compute::task> *messaging::memory::memory::getMessageQueue()
{
	return messages;
}

::queue::chain_factory<data::response::response> *messaging::memory::memory::getResponsesQueue()
{
	return responses;
}

void messaging::memory::memory::makeNull()
{
	messages = NULL;
	responses = NULL;
}

void messaging::memory::memory::cleanup()
{
	if (responses != NULL) delete responses;
	if (messages != NULL) delete messages;
}

void messaging::database::database::reset(::database::settings &settings)
{
	init = false; cleanup();

	messages = new queues::database::incoming::factory(settings);
	if (messages == NULL) return;
	if (!messages->initalised()) return;

	responses = new queues::database::outgoing::factory(settings);
	if (responses == NULL) return;
	if (!responses->initalised()) return;

	init = true;
}

::queue::factory<compute::task> *messaging::database::database::getMessageQueue()
{
	return messages;
}

::queue::chain_factory<data::response::response> *messaging::database::database::getResponsesQueue()
{
	return responses;
}

void messaging::database::database::makeNull()
{
	messages = NULL;
	responses = NULL;
}

void messaging::database::database::cleanup()
{
	if (responses != NULL) delete responses;
	if (messages != NULL) delete messages;
}