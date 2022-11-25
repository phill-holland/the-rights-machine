#include "parser/json/parser.h"
#include "server/configuration.h"
#include "compute/manager.h"
#include "queues/memory.h"
#include "compute/cpu.h"
#include "server/server.h"
#include "server/errors.h"
#include "database/records.h"
#include "database/users.h"
#include "server/settings.h"
#include "console.h"

#if !defined(__STARTER)
#define __STARTER

namespace server
{
	class starter
	{
		database::factory::connection *connections;
		database::factory::recordset *recordsets;

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