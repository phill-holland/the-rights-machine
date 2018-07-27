#include "fifo.h"
#include "queue.h"
#include "message.h"
#include "database.h"
#include "response.h"
#include "task.h"
#include "factory.h"

#if !defined(__QUEUES)
#define __QUEUES

namespace queues
{
	namespace memory
	{
		namespace incoming
		{
			class queue : public custom::fifo<compute::task, 10L> { };

			class factory : public ::queue::factory<compute::task>
			{
				std::vector<::queue::queue<compute::task>*> queues;

				bool init;

			public:
				factory() { makeNull(); reset(); }
				~factory() { cleanup(); }

				bool initalised() { return init; }
				void reset();

				::queue::queue<compute::task> *get();

			protected:
				void makeNull();
				void cleanup();
			};
		};

		namespace outgoing
		{
			class queue : public custom::fifo<data::response::response, 10L> { };

			class factory : public ::queue::factory<data::response::response>
			{
				std::vector<::queue::queue<data::response::response>*> queues;

				bool init;

			public:
				factory() { makeNull(); reset(); }
				~factory() { cleanup(); }

				bool initalised() { return init; }
				void reset();

				::queue::queue<data::response::response> *get();

			protected:
				void makeNull();
				void cleanup();
			};
		};
	};

	namespace database
	{
		namespace incoming
		{
			class queue : public ::queue::queue<data::message::message>
			{
			protected:
				static const long LENGTH = 100L;

				const static unsigned long INPUT = 15L;
				const static unsigned long OUTPUT = 15L;

			private:
				custom::fifo<data::message::message, LENGTH> *incoming;
				custom::fifo<data::message::message, LENGTH> *outgoing; // fetch buffer to reduce database hits

				bool init;

			public:
				queue(::database::connection *connection) { makeNull(); reset(connection); }
				~queue() { cleanup(); }

				bool initalised() { return init; }
				void reset(::database::connection *connection);

				bool get(data::message::message &destination);
				bool set(data::message::message &source);

				bool flush();

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