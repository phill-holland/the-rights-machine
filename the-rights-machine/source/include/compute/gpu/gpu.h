#include "compute/interfaces/compute.h"
#include "compute/interfaces/grid.h"
#include "compute/interfaces/row.h"
#include "compute/task.h"
#include "compute/gpu/processor.h"
#include "queues/interfaces/queue.h"
#include "interfaces/factory.h"
#include "message/message.h"
#include "models/response.h"
#include "models/line.h"
#include "core/queue/fifo.h"
#include "core/threading/thread.h"
#include "grid.cuh"
#include "row.cuh"

#if !defined(_COMPUTE_GPU)
#define _COMPUTE_GPU

namespace compute
{
	namespace gpu
	{
		class gpu : public compute, public core::threading::thread
		{
			::queue::queue<::compute::task> *queue;
			processor *process;

			bool init;

		public:
			void background(thread *bt);

		public:
			gpu(::queue::factory<::compute::task> *factory) { makeNull(); reset(factory); }
			~gpu() { cleanup(); }

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