#include "compute/interfaces/compute.h"
#include "compute/interfaces/grid.h"
#include "compute/task.h"
#include "compute/grid.h"
#include "queues/interfaces/queue.h"
#include "message/message.h"
#include "models/response.h"
#include "core/queue/fifo.h"
#include "core/threading/thread.h"
#include "interfaces/factory.h"
#include "models/line.h"

#if !defined(_COMPUTE_CPU_PROCESSOR)
#define _COMPUTE_CPU_PROCESSOR

namespace compute
{
	namespace cpu
	{
		class processor
		{
			const static unsigned long WIDTH = 255;
			const static unsigned long HEIGHT = 255;

		private:
			unsigned long width, height;
			unsigned long input_ptr, output_ptr;

			::compute::cpu::grid *in, *out, *query;
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