#include "configuration.h"

void server::configuration::configuration::reset(manager::manager *manager, error::errors *errors)
{
	this->manager = manager;
	this->errors = errors;

	port = PORT;
	clients = CLIENTS;
}

void server::configuration::configuration::copy(configuration const &source)
{
	manager = source.manager;
	errors = source.errors;
	port = source.port;
	clients = source.clients;
}