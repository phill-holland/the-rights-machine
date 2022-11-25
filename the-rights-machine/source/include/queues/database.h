#include "core/queue/fifo.h"
#include "queues/interfaces/queue.h"
#include "database/databases.h"
#include "message/message.h"
#include "database/databases.h"
#include "models/response.h"
#include "compute/task.h"
#include "interfaces/factory.h"
#include "interfaces/chain.h"
#include "models/responses.h"
#include "core/threading/thread.h"
#include "database/storage.h"
#include <stdio.h>
#include <stdlib.h>

#if !defined(__DATABASE)
#define __DATABASE

namespace queues
{
	namespace database
	{
		namespace incoming
		{
			class queue : public ::queue::queue<compute::task>, public core::threading::thread
			{
			protected:
				static const unsigned long INTERVAL = 100UL;

				static const long LENGTH = 100L;

				const static unsigned long INPUT = 15L;
				const static unsigned long OUTPUT = 15L;

			private:
				fifo<compute::task, LENGTH> *incoming;
				fifo<compute::task, LENGTH> *outgoing;

				mutex::token polling, flushing;

				unsigned long counter, interval;

				::database::settings *settings;
				::database::storage::message *message;

				bool init;

			public:
				void background(thread *bt);

			public:
				queue(::database::settings &settings, unsigned long interval = INTERVAL) { makeNull(); reset(settings, interval); }
				~queue() { cleanup(); }

				bool initalised() { return init; }
				void reset(::database::settings &settings, unsigned long interval = INTERVAL);

				bool get(compute::task &destination);
				bool set(compute::task &source);

			protected:
				bool flush();
				bool poll();

			protected:
				void makeNull();
				void cleanup();
			};

			class factory : public ::queue::factory<compute::task>
			{
				const static unsigned long MAX = 10UL;

			private:
				::queue::queue<compute::task> **queues;
				::database::settings *settings;

				unsigned long total;
				unsigned long length;

				bool init;

			public:
				factory(::database::settings &settings, unsigned long total = MAX) { makeNull(); reset(settings, total); }
				~factory() { cleanup(); }

				bool initalised() { return init; }
				void reset(::database::settings &settings, unsigned long total);

				::queue::queue<compute::task> *get();

			protected:
				void makeNull();
				void cleanup();
			};
		};

		namespace outgoing
		{
			class queue : public ::queue::queue<data::response::response>, public thread
			{
			protected:
				static const unsigned long INTERVAL = 100UL;

				static const long LENGTH = 100L;

				const static unsigned long INPUT = 15L;
				const static unsigned long OUTPUT = 15L;

			private:
				fifo<data::response::response, LENGTH> *incoming;
				fifo<data::response::response, LENGTH> *outgoing;

				mutex::token polling, flushing;

				unsigned long counter, interval;

				::database::settings *settings;
				::database::storage::response *response;

				bool init;

			public:
				void background(thread *bt);

			public:
				queue(::database::settings &settings, unsigned long interval = INTERVAL) { makeNull(); reset(settings, interval); }
				~queue() { cleanup(); }

				bool initalised() { return init; }
				void reset(::database::settings &settings, unsigned long interval = INTERVAL);

				bool get(data::response::response &destination);
				bool set(data::response::response &source);

			protected:
				bool flush();
				bool poll();

			protected:
				void makeNull();
				void cleanup();
			};

			class factory : public ::queue::chain_factory<data::response::response>
			{
				const static unsigned long MAX = 10UL;

			private:
				data::response::responses **queues;

				unsigned long total;
				unsigned long length;

				bool init;

			public:
				factory(::database::settings &settings, unsigned long total = MAX) { makeNull(); reset(settings, total); }
				~factory() { cleanup(); }

				bool initalised() { return init; }
				void reset(::database::settings &settings, unsigned long total);

				::custom::chain<data::response::response> *get();

			protected:
				void makeNull();
				void cleanup();
			};			
		};
	};
};

#endif