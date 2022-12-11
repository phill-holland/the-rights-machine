#include "queues/interfaces/queue.h"
#include "core/queue/fifo.h"
#include "queues/interfaces/queue.h"
#include "message/message.h"
#include "models/response.h"
#include "models/responses.h"
#include "compute/task.h"
#include "interfaces/factory.h"
#include "interfaces/chain.h"

#if !defined(__MEMORY)
#define __MEMORY

namespace queues
{
	namespace memory
	{
		namespace incoming
		{
			class queue : public ::queue::queue<compute::task>, public core::queue::fifo<compute::task, 10L>
			{ 
				public:
					bool flush()
					{
						return true;
					}

					bool set(compute::task &source) 
					{
						return core::queue::fifo<compute::task, 10L>::set(source);
					}

					bool get(compute::task &destination)
					{
						return core::queue::fifo<compute::task, 10L>::get(destination);
					}
			};

			class factory : public ::queue::factory<compute::task>
			{
				const static unsigned long MAX = 10UL;

			private:
				::queue::queue<compute::task> **queues;

				unsigned long total;
				unsigned long length;

				bool init;

			public:
				factory(unsigned long total = MAX) { makeNull(); reset(total); }
				~factory() { cleanup(); }

				bool initalised() { return init; }
				void reset(unsigned long total);

				::queue::queue<compute::task> *get();

			protected:
				void makeNull();
				void cleanup();
			};
		};

		namespace outgoing
		{
			class factory : public ::queue::chain_factory<models::response::response>
			{
				const static unsigned long MAX = 10UL;

			private:
				models::response::responses **queues;

				unsigned long total;
				unsigned long length;

				bool init;

			public:
				factory(unsigned long total = MAX) { makeNull(); reset(total); }
				~factory() { cleanup(); }

				bool initalised() { return init; }
				void reset(unsigned long total);

				::custom::chain<models::response::response> *get();

			protected:
				void makeNull();
				void cleanup();
			};
		};
	};
};

#endif