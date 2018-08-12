#include "wsock.h"
#include <windows.h>
#include "configuration.h"
#include "manager.h"
#include "queues.h"
#include "cpu.h"
#include "server.h"
#include "errors.h"

#if !defined(__STARTER)
#define __STARTER

namespace server
{
	class starter
	{
		queues::memory::incoming::factory *messages;
		queues::memory::outgoing::factory *responses;

		compute::cpu::cpu *cpu;

		manager::manager *manager;

		error::console::errors *console;
		error::errors *errors;
			
		::server::configuration::configuration *configuration;
		::server::server *server;

		bool init;

	public:
		starter() { makeNull(); reset(); }
		~starter() { cleanup(); }

		bool initalised() { return init; }
		void reset();

		bool start();
		void stop();

	protected:
		void makeNull();
		void cleanup();
	};
};

#endif