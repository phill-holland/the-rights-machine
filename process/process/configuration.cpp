#include "configuration.h"

void server::configuration::reset()
{
	port = PORT;
	clients = CLIENTS;
	output = NULL;
}

void server::configuration::copy(configuration const &source)
{
	port = source.port;
	clients = source.clients;
	output = source.output;
}