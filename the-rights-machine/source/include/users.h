#include "string.h"
#include "user.h"
#include "thread.h"
#include "mutex.h"
#include "databases.h"
#include "comparison.h"
#include <unordered_map>

#if !defined(__USERS)
#define __USERS

namespace data
{
	using namespace comparison;

	class users : public thread
	{
	protected:
		static const unsigned long INTERVAL = 100UL;

	private:
		std::unordered_map<string, data::user, hasher, equality> map;

		mutex::token token;

		string location;
		unsigned long counter, interval;

		database::connection *connection;
		database::recordset *recordset;

		bool first;
		bool init;

	public:
		DWORD WINAPI background(thread *bt);

	public:
		users(database::settings &settings, unsigned long interval = INTERVAL) { makeNull(); reset(settings, interval); }
		~users() { cleanup(); }

		bool initalised() { return init; }

		void reset(database::settings &settings, unsigned long interval);

		data::user get(string user);

	protected:
		void refresh();

	protected:
		void makeNull();
		void cleanup();
	};
};

#endif