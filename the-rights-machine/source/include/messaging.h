#include "memory.h"
#include "database.h"
#include "response.h"

#if !defined(__MESSAGING)
#define __MESSAGING

namespace messaging
{
	namespace common
	{
		class messaging
		{
		public:
			virtual ::queue::factory<compute::task> *getMessageQueue() = 0;
			virtual ::queue::chain_factory<data::response::response> *getResponsesQueue() = 0;
		};
	};

	namespace memory
	{
		class memory : public common::messaging
		{
			queues::memory::incoming::factory *messages;
			queues::memory::outgoing::factory  *responses;

			bool init;

		public:
			memory() { makeNull(); reset(); }
			~memory() { cleanup(); }

			bool initalised() { return init; }
			void reset();

			::queue::factory<compute::task> *getMessageQueue();
			::queue::chain_factory<data::response::response> *getResponsesQueue();

		protected:
			void makeNull();
			void cleanup();
		};
	};

	namespace database
	{
		class database : public common::messaging
		{
			queues::database::incoming::factory *messages;
			queues::database::outgoing::factory *responses;

			bool init;

		public:
			database(::database::settings &settings) { makeNull(); reset(settings); }
			~database() { cleanup(); }

			bool initalised() { return init; }
			void reset(::database::settings &settings);

			::queue::factory<compute::task> *getMessageQueue();
			::queue::chain_factory<data::response::response> *getResponsesQueue();

		protected:
			void makeNull();
			void cleanup();
		};
	};
};

#endif
