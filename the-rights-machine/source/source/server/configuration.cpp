#include "server/configuration.h"

void configuration::server::configuration::reset(manager::manager *manager, data::users *users, error::errors *errors)
{
	this->manager = manager;
	this->users = users;
	this->errors = errors;

	port = PORT;
	clients = CLIENTS;
}

void configuration::server::configuration::copy(configuration const &source)
{
	manager = source.manager;
	users = source.users;
	errors = source.errors;
	port = source.port;
	clients = source.clients;
}

void configuration::server::client::configuration::reset(manager::manager *manager,
														 custom::chain<models::response::response> *responses,
														 pending::pending *requested,
														 data::users *users,
														 error::errors *errors)
{
	this->manager = manager;
	this->responses = responses;
	this->requested = requested;
	this->users = users;
	this->errors = errors;
}

void configuration::server::client::configuration::copy(configuration const &source)
{
	manager = source.manager;
	responses = source.responses;
	requested = source.requested;
	users = source.users;
	errors = source.errors;
}

void configuration::server::client::configuration::copy(::configuration::server::configuration const &source, pending::pending *requested)
{
	manager = source.manager;
	responses = source.manager->get();
	this->requested = requested;
	users = source.users;
	errors = source.errors;
}
