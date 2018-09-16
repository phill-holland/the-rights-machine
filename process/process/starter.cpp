#include "starter.h"
#include "log.h"

void server::starter::reset(string location)
{
	init = false; cleanup();

	messages = new queues::memory::incoming::factory();
	if (messages == NULL) return;
	if (!messages->initalised()) return;

	responses = new queues::memory::outgoing::factory();
	if (responses == NULL) return;
	if (!responses->initalised()) return;

	connections = new database::odbc::factory::connection();
	if (connections == NULL) return;
	if (!connections->initalised()) return;

	recordsets = new database::odbc::factory::recordset();
	if (recordsets == NULL) return;
	if (!recordsets->initalised()) return;

	cpu = new compute::cpu::cpu(messages);
	if (cpu == NULL) return;
	if (!cpu->initalised()) return;
	
	manager = new manager::manager(responses);
	if (manager == NULL) return;
	if (!manager->initalised()) return;
	manager->add(cpu);

	console = new error::console::errors();
	if (console == NULL) return;
	if (!console->initalised()) return;

	errors = new error::errors(console);
	if (errors == NULL) return;
	if (!errors->initalised()) return;

	database::settings settings(location, connections, recordsets);
	if (!settings.initalised()) return;

	users = new data::users(settings);
	if (users == NULL) return;
	if (!users->initalised()) return;

	configuration = new ::configuration::server::configuration(manager, users, errors);
	if (configuration == NULL) return;

	server = new ::server::server(configuration);
	if (server == NULL) return;
	if (!server->initalised()) return;

	init = true;
}

bool server::starter::start()
{
	if (!cpu->start()) return false;
	if (!users->start()) return false;
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
	users->stop();
	cpu->stop();
}

void server::starter::shutdown()
{
	server->shutdown();
	errors->shutdown();
	users->stop();
	cpu->shutdown();
}

void server::starter::makeNull()
{
	messages = NULL;
	responses = NULL;
	connections = NULL;
	recordsets = NULL;
	cpu = NULL;
	manager = NULL;
	console = NULL;
	errors = NULL;
	users = NULL;
	configuration = NULL;
	server = NULL;
}

void server::starter::cleanup()
{
	if (server != NULL) delete server;
	if (configuration != NULL) delete configuration;
	if (users != NULL) delete users;
	if (errors != NULL) delete errors;
	if (console != NULL) delete console;
	if (manager != NULL) delete manager;
	if (cpu != NULL) delete cpu;
	if (recordsets != NULL) delete recordsets;
	if (connections != NULL) delete connections;
	if (responses != NULL) delete responses;
	if (messages != NULL) delete messages;
}