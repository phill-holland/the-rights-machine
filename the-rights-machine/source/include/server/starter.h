#include "parser/json/parser.h"
#include "queues/memory.h"
#include "compute/manager.h"
#include "compute/cpu/cpu.h"
#include "server/server.h"
#include "server/errors.h"
#include "server/settings.h"
#include "server/console.h"
#include "server/configuration.h"
#include "database/records.h"
#include "database/users.h"
#include "core/database/factory/connection.h"
#include "core/database/factory/recordset.h"

#if !defined(__STARTER)
#define __STARTER

namespace server
{
	class starter
	{
		core::database::factory::connection *connections;
		core::database::factory::recordset *recordsets;

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