#include "server/configuration.h"
#include "compute/manager.h"
#include "queues/memory.h"
#include "server/server.h"
#include "server/starter.h"
#include "server/error.h"

void startup()
{
	const long port = 5454;

	messaging::memory::memory *messaging = NULL;
	server::settings *setup = NULL;
	server::starter *starter = NULL;

	messaging = new messaging::memory::memory();
	if(messaging != NULL) 
	{
		setup = new server::settings(messaging, port);
		if(setup != NULL)
		{
			starter = new server::starter(*setup);
			if(starter != NULL)
			{
				if(starter->initalised())
				{
					if (starter->start())
					{
						while (1)
						{
							std::this_thread::sleep_for(std::chrono::milliseconds(500));
						};
					}
				}

				delete starter;
			}

			delete setup;
		}

		delete messaging;
	}
}

int main(int argc, char **argv)
{
	startup();

	return 0;
}