#include "compute.h"
#include "queue.h"
#include "message.h"
#include "response.h"
#include "fifo.h"
#include "task.h"
#include "factory.h"
#include "thread.h"

#include "line.h"

#if !defined(__CPU)
#define __CPU

namespace compute
{
	namespace cpu
	{
		class header
		{
		public:
			int messageID;
			int itemID;
			int lineID;
		};

		class block
		{
		public:
			header headers[255];
			int data[255][255];

			data::line::line in[255], out[255];

		public:
			
			void split(void *lines)
			{
				// loop through lines, add to in or out array


			}

		};

		/*
		class inlines
		{
		public:
			int lineID; // have two lines after a split with same ID
			datetime start, end;

		};
		*/
		class cpu : public compute, public thread
		{
			::queue::queue<::compute::task> *queue;

			bool init;

		protected:
			void sort(datetime &a, datetime &b, datetime &c, datetime &d);

		public:
			DWORD WINAPI background(thread *bt);

		public:
			cpu(::queue::factory<::compute::task> *factory) { makeNull(); reset(factory); }
			~cpu() { cleanup(); }

			bool initalised() { return init; }
			void reset(::queue::factory<::compute::task> *factory);

			bool set(::compute::task &source) { return queue->set(source); }
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