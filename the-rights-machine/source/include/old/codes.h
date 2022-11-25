#include "error.h"
#include "queues/interfaces/queue.h"
#include "core/string/string.h"
#include "core/threading/thread.h"
#include "core/threading/mutex.h"
#include "database/interfaces/databases.h"
#include <vector>

#if !defined(__CODES)
#define __CODES

namespace error
{
	class codes : public queue::out<::error::type::type>
	{
	private:
		core::threading::mutex::token token;

		string location;

		database::connection *connection;
		database::recordset *recordset;

		std::vector<::error::type::type> data;

		bool init;

	public:
		codes(database::settings &settings) { makeNull(); reset(settings); }
		~codes() { cleanup(); }

		bool initalised() { return init; }

		void reset(database::settings &settings);

		bool get(::error::type::type &destination);

	protected:
		void refresh();

	protected:
		void makeNull();
		void cleanup();
	};
};

#endif