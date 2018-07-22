#include "compute.h"
#include "queue.h"
#include "message.h"
#include "response.h"
#include "fifo.h"
#include "task.h"
#include "factory.h"
#include "thread.h"
#include "line.h"
#include "grid.h"

#if !defined(__CPU)
#define __CPU

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
			
			grid *in, *out, *query;
			row **rows;

			data::line::line *inputs;
			data::line::line *outputs;
			
			bool init;

		public:
			processor(unsigned long width, unsigned long height) { makeNull(); reset(width, height); }
			~processor() { cleanup(); }
			
			bool initalised() { return init; }
			void reset(unsigned long width, unsigned long height);

			void clear();
			
			void push(data::message::message &message);

		protected:
			void makeNull();
			void cleanup();
		};

		class cpu : public compute, public thread
		{
			::queue::queue<::compute::task> *queue;

			bool init;

		protected:
			void process();

		public:
			DWORD WINAPI background(thread *bt);

		public:
			cpu(::queue::factory<::compute::task> *factory) { makeNull(); reset(factory); }
			~cpu() { cleanup(); }

			bool initalised() { return init; }
			void reset(::queue::factory<::compute::task> *factory);

			bool set(::compute::task &source) 
			{ 
				return queue->set(source); 
				process();
			}
			bool flush() { return queue->flush(); }

		protected:
			bool get(::compute::task &destination) { return queue->get(destination); }

		protected:
			void makeNull();
			void cleanup();
		};
	};
};

#endif