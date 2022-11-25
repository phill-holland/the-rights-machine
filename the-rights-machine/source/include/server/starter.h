#include "parser/json/parser.h"
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
#include "settings.h"
#include "console.h"

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

		debug::console *debugging;
		error::errors *errors;

		::configuration::server::configuration *configuration;
		::server::server *server;

		data::users *users;

		bool init;

	public:
		starter(settings &setup) { makeNull(); reset(setup); }
		~starter() { cleanup(); }

		bool initalised() { return init; }
		void reset(settings &setup);

		bool start();
		void stop();

		void shutdown();

	protected:
		void makeNull();
		void cleanup();
	};
};

#endif