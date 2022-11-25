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
				core::queue::fifo<compute::task, LENGTH> *incoming;
				core::queue::fifo<compute::task, LENGTH> *outgoing;

				core::threading::mutex::token polling, flushing;

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
			class queue : public ::queue::queue<models::response::response>, public core::threading::thread
			{
			protected:
				static const unsigned long INTERVAL = 100UL;

				static const long LENGTH = 100L;

				const static unsigned long INPUT = 15L;
				const static unsigned long OUTPUT = 15L;

			private:
				core::queue::fifo<models::response::response, LENGTH> *incoming;
				core::queue::fifo<models::response::response, LENGTH> *outgoing;

				core::threading::mutex::token polling, flushing;

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

				bool get(models::response::response &destination);
				bool set(models::response::response &source);

			protected:
				bool flush();
				bool poll();

			protected:
				void makeNull();
				void cleanup();
			};

			class factory : public ::queue::chain_factory<models::response::response>
			{
				const static unsigned long MAX = 10UL;

			private:
				models::response::responses **queues;

				unsigned long total;
				unsigned long length;

				bool init;

			public:
				factory(::database::settings &settings, unsigned long total = MAX) { makeNull(); reset(settings, total); }
				~factory() { cleanup(); }

				bool initalised() { return init; }
				void reset(::database::settings &settings, unsigned long total);

				::custom::chain<models::response::response> *get();

			protected:
				void makeNull();
				void cleanup();
			};			
		};
	};
};

#endif