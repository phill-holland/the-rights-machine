#include "error.h"
#include "queue.h"
#include "string.h"
#include "thread.h"
#include "mutex.h"
#include "databases.h"
#include <vector>

#if !defined(__CODES)
#define __CODES

namespace error
{
	class codes : public queue::out<::error::type::type>
	{
	private:
		mutex::token token;

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