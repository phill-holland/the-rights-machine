#include "fifo.h"
#include "queue.h"
#include "databases.h"
#include "message.h"
#include "databases.h"
#include "response.h"
#include "task.h"
#include "factory.h"
#include "chain.h"
#include "responses.h"
#include "thread.h"

#if !defined(__DATABASE)
#define __DATABASE

namespace queues
{
	namespace database
	{
		namespace incoming
		{
			class queue : public ::queue::queue<data::message::message>, public thread
			{
			protected:
				static const unsigned long INTERVAL = 100UL;

				static const long LENGTH = 100L;

				const static unsigned long INPUT = 15L;
				const static unsigned long OUTPUT = 15L;

			private:
				custom::fifo<data::message::message, LENGTH> *incoming;
				custom::fifo<data::message::message, LENGTH> *outgoing; // fetch buffer to reduce database hits

				mutex::token token;

				string location;
				unsigned long counter, interval;

				::database::connection *connection;
				::database::recordset *recordset;

				bool init;

			public:
				DWORD WINAPI background(thread *bt);

			public:
				queue(::database::settings &settings, unsigned long interval = INTERVAL) { makeNull(); reset(settings, interval); }
				~queue() { cleanup(); }

				bool initalised() { return init; }
				void reset(::database::settings &settings, unsigned long interval = INTERVAL);

				bool get(data::message::message &destination);
				bool set(data::message::message &source);

				bool flush();

			protected:
				bool write();
				bool read();

			protected:
				void makeNull();
				void cleanup();
			};
		};

		namespace outgoing
		{
			class queue : public ::queue::queue<data::response::response>
			{
			public:
				bool get(data::response::response &destination) { return false; }
				bool set(data::response::response &source) { return false; }

				bool flush() { return false; }

				// pass this into server class too!!!

				// then use HTTP header GET/POST to determine which queue the user wants!!
			};
		};
	};
};

#endif