#include "databases.h"
#include "thread.h"
#include "mutex.h"

namespace watchdog
{
	class watchdog : public thread
	{
		static const unsigned long INTERVAL = 300UL;

	private:
		string location;

		unsigned long counter, interval;

		database::connection *connection;
		database::recordset *recordset;

		bool init;

	public:
		void background(thread *bt);

	public:
		watchdog(database::settings &settings, unsigned long interval = INTERVAL) { makeNull(); reset(settings, interval); }
		~watchdog() { cleanup(); }

		bool initalised() { return init; }
		void reset(database::settings &settings, unsigned long interval);
	
	protected:
		void refresh();

	protected:
		void makeNull();
		void cleanup();
	};
};