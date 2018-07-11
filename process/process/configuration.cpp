#include "configuration.h"

void server::configuration::reset(manager::manager *manager)
{
	this->manager = manager;

	port = PORT;
	clients = CLIENTS;
}

void server::configuration::copy(configuration const &source)
{
	manager = source.manager;
	port = source.port;
	clients = source.clients;
}