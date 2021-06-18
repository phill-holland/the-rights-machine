#include "configuration.h"
#include "manager.h"
#include "memory.h"
#include "cpu.h"
#include "server.h"
#include "errors.h"
#include "log.h"

#include "element.h"
#include "elements.h"
#include "guid.h"
#include "starter.h"

#include "grid.h"
#include "error.h"
#include "crumbs.h"
#include "parameters.h"

#include "users.h"
#include "odbc.h"

void test()
{
	queues::memory::incoming::factory messages;
	queues::memory::outgoing::factory responses;

	compute::cpu::cpu cpu(&messages);
	if (!cpu.start()) return;

	manager::manager manager(&responses);
	manager.add(&cpu);

	error::console::errors console;
	error::errors errors(&console);
	if (!errors.start()) return;


	server::configuration::configuration configuration(&manager, &errors);
	server::server *server = new server::server(&configuration);

	if (server == NULL) return;
	if (server->initalised())
	{
		if (server->open())
		{
			if (server->start())
			{
				while (1)
				{
					std::this_thread::sleep_for(std::chrono::milliseconds(500));
				};
			}
		}
	}

	delete server;
}

int main(int argc, char **argv)
{
	test();

	//curl --header "Content-Type: application/json"  --request POST --data-binary @body.json http://127.0.0.1:5555

	return 0;
}