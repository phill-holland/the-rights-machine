#include "wsock.h"
#include <windows.h>
#include "configuration.h"
#include "manager.h"
#include "memory.h"
#include "cpu.h"
#include "server.h"
#include "errors.h"
#include "odbc.h"
#include "users.h"

#if !defined(__STARTER)
#define __STARTER

namespace server
{
	class starter
	{
		queues::memory::incoming::factory *messages;
		queues::memory::outgoing::factory *responses;

		database::odbc::factory::connection *connections;
		database::odbc::factory::recordset *recordsets;

		compute::cpu::cpu *cpu;

		manager::manager *manager;

		error::console::errors *console;
		error::errors *errors;
			
		::configuration::server::configuration *configuration;
		::server::server *server;

		data::users *users;

		bool init;

	public:
		starter(string location) { makeNull(); reset(location); }
		~starter() { cleanup(); }

		bool initalised() { return init; }
		void reset(string location);

		bool start();
		void stop();

		void shutdown();

	protected:
		void makeNull();
		void cleanup();
	};
};

#endif