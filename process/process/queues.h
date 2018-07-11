#include "fifo.h"
#include "queue.h"
#include "message.h"
#include "database.h"
#include "response.h"
#include "factory.h"

#if !defined(__QUEUES)
#define __QUEUES

namespace queues
{
	namespace memory
	{
		namespace incoming
		{
			class queue : public ::queue::queue<data::message::message>
			{
			protected:
				static const long LENGTH = 100L;

			private:
				// this number needs to be variable??? configurable??
				custom::fifo<data::message::message, LENGTH> *buffer; // fifo<data::line> is a member of data::item
				bool init;

			public:
				queue() { makeNull(); reset(); }
				~queue() { cleanup(); }

				bool initalised() { return init; }

				void reset();

				bool get(data::message::message &destination);
				bool set(data::message::message &source);

				bool flush() { return true; }

			protected:
				void makeNull();
				void cleanup();
			};

			class factory : public ::queue::factory<data::message::message>
			{
			public:
				queue *get()
				{
					return NULL;
				}
			};
		};

		namespace outgoing
		{
			class queue : public ::queue::queue<data::response>
			{
			public:
				bool get(data::response &destination) { return false; }
				bool set(data::response &source) { return false; }

				bool flush() { return false; }

				// pass this into server class too!!!

				// then use HTTP header GET/POST to determine which queue the user wants!!
			};

			class factory : public ::queue::factory<data::response>
			{
			public:
				queue *get()
				{
					return NULL;
				}
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
			class queue : public ::queue::queue<data::response>
			{
			public:
				bool get(data::response &destination) { return false; }
				bool set(data::response &source) { return false; }

				bool flush() { return false; }

				// pass this into server class too!!!

				// then use HTTP header GET/POST to determine which queue the user wants!!
			};
		};
	};
};

#endif