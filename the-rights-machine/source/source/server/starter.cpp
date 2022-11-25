#include "server/starter.h"

void server::starter::reset(settings &setup)
{
	init = false; cleanup();

	connections = new database::factory::connection();
	if (connections == NULL) return;
	if (!connections->initalised()) return;

	recordsets = new database::factory::recordset();
	if (recordsets == NULL) return;
	if (!recordsets->initalised()) return;

	cpu = new compute::cpu::cpu(setup.messaging->getMessageQueue());
	if (cpu == NULL) return;
	if (!cpu->initalised()) return;

	manager = new manager::manager(setup.messaging->getResponsesQueue());
	if (manager == NULL) return;
	if (!manager->initalised()) return;
	manager->add(cpu);

	//debugging = new debug::console();
	//if(debugging == NULL) return;
	//if(!debugging->initalised()) return;

	errors = new error::errors(debugging);
	if (errors == NULL) return;
	if (!errors->initalised()) return;

	database::settings settings(setup.connection, connections, recordsets);
	if (!settings.initalised()) return;

	users = new data::users(settings);
	if (users == NULL) return;
	if (!users->initalised()) return;

	configuration = new ::configuration::server::configuration(manager, users, errors);
	if (configuration == NULL) return;

	configuration->port = setup.port;
	configuration->clients = setup.clients;

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
	connections = NULL;
	recordsets = NULL;
	cpu = NULL;
	manager = NULL;
	//debugging = NULL;
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
	//if (debugging != NULL) delete debugging;
	if (manager != NULL) delete manager;
	if (cpu != NULL) delete cpu;
	if (recordsets != NULL) delete recordsets;
	if (connections != NULL) delete connections;
}