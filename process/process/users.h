#include "string.h"
#include "user.h"
#include "thread.h"
#include "mutex.h"
#include "database.h"
#include <unordered_map>

#if !defined(__USERS)
#define __USERS

namespace data
{
	class users : public thread
	{
		std::unordered_map<int, data::user> map;

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
		users(unsigned long interval, database::settings &settings) { makeNull(); reset(interval, settings); }
		~users() { cleanup(); }

		bool initalised() { return init; }

		void reset(unsigned long interval, database::settings &settings);

		data::user *get(int id)
		{
			mutex lock(token);

			if (map.find(id) == map.end()) return NULL;
			data::user *result = &map[id];
			if (!result->validate()) return NULL;

			return result;
		}

	protected:
		void refresh();

	protected:
		void makeNull();
		void cleanup();
	};
};

#endif