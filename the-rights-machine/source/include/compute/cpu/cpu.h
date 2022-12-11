#include "compute/interfaces/compute.h"
#include "compute/interfaces/grid.h"
#include "compute/task.h"
#include "compute/grid.h"
#include "compute/cpu/processor.h"
#include "queues/interfaces/queue.h"
#include "message/message.h"
#include "models/response.h"
#include "core/queue/fifo.h"
#include "core/threading/thread.h"
#include "interfaces/factory.h"
#include "models/line.h"

#if !defined(_COMPUTE_CPU)
#define _COMPUTE_CPU

namespace compute
{
	namespace cpu
	{
		class cpu : public compute, public core::threading::thread
		{
			::queue::queue<::compute::task> *queue;
			processor *process;

			bool init;

		public:
			void background(thread *bt);

		public:
			cpu(::queue::factory<::compute::task> *factory) { makeNull(); reset(factory); }
			~cpu() { cleanup(); }

			bool initalised() { return init; }
			void reset(::queue::factory<::compute::task> *factory);

			bool set(::compute::task &source)
			{
				return queue->set(source);
			}

			bool flush() { return queue->flush(); }

			bool shutdown() { return stopAndWait(); }

		protected:
			bool get(::compute::task &destination) { return queue->get(destination); }

		protected:
			void makeNull();
			void cleanup();
		};
	};
};

#endif