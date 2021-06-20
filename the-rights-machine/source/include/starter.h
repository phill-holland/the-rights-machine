#include "wsock.h"

#include "configuration.h"
#include "manager.h"
#include "memory.h"
#include "cpu.h"
#include "server.h"
#include "errors.h"
#include "odbc.h"
#include "users.h"
#include "messaging.h"

#if !defined(__STARTER)
#define __STARTER

namespace server
{
	class starter
	{
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
		starter(messaging::common::messaging *messaging) { makeNull(); reset(messaging); }
		~starter() { cleanup(); }

		bool initalised() { return init; }
		void reset(messaging::common::messaging *messaging);

		bool start();
		void stop();

		void shutdown();

	protected:
		void makeNull();
		void cleanup();
	};
};

#endif