#include "starter.h"

void server::starter::reset()
{
	init = false; cleanup();

	messages = new queues::memory::incoming::factory();
	if (messages == NULL) return;

	responses = new queues::memory::outgoing::factory();
	if (responses == NULL) return;

	cpu = new compute::cpu::cpu(messages);
	if (cpu == NULL) return;

	manager = new manager::manager(responses);
	if (manager == NULL) return;
	manager->add(cpu);

	console = new error::console::errors();
	if (console == NULL) return;
	errors = new error::errors(console);

	configuration = new ::server::configuration::configuration(manager, errors);
	if (configuration == NULL) return;

	server = new ::server::server(configuration);
	if (server == NULL) return;

	init = true;
}

bool server::starter::start()
{
	if (!cpu->start()) return false;
	if (!errors->start()) return false;
	
	if (!server->open()) return false;
	if (!server->start()) return false;

	return true;
}

void server::starter::stop()
{
	if (server->isopen()) server->close();
	server->stop();
	errors->stop();
	cpu->stop();
}

void server::starter::makeNull()
{
	messages = NULL;
	responses = NULL;
	cpu = NULL;
	manager = NULL;
	console = NULL;
	errors = NULL;
	configuration = NULL;
	server = NULL;
}

void server::starter::cleanup()
{
	if (server != NULL) delete server;
	if (configuration != NULL) delete configuration;
	if (errors != NULL) delete errors;
	if (console != NULL) delete console;
	if (manager != NULL) delete manager;
	if (cpu != NULL) delete cpu;
	if (responses != NULL) delete responses;
	if (messages != NULL) delete messages;
}