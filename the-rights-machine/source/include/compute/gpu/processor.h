#include "compute/interfaces/compute.h"
#include "compute/interfaces/grid.h"
#include "compute/interfaces/row.h"
#include "compute/task.h"
#include "queues/interfaces/queue.h"
#include "interfaces/factory.h"
#include "message/message.h"
#include "models/response.h"
#include "models/line.h"
#include "core/queue/fifo.h"
#include "core/threading/thread.h"
#include "grid.cuh"
#include "row.cuh"

#if !defined(_COMPUTE_GPU_PROCESSOR)
#define _COMPUTE_GPU_PROCESSOR

namespace compute
{
	namespace gpu
	{
		class processor
		{
			const static unsigned long WIDTH = 255;
			const static unsigned long HEIGHT = 255;

		private:
			unsigned long width, height;
			unsigned long input_ptr, output_ptr;

			::compute::gpu::grid *in, *out, *query;
			::compute::interfaces::row **rows;

			models::line::line *inputs;
			models::line::line *outputs;

			bool init;

		public:
			processor(unsigned long width, unsigned long height) { makeNull(); reset(width, height); }
			~processor() { cleanup(); }

			bool initalised() { return init; }
			void reset(unsigned long width, unsigned long height);

			void clear();

			void push(::compute::task &task);

		protected:
			void makeNull();
			void cleanup();
		};
	};
};


#endif