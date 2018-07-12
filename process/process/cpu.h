#include "compute.h"
#include "queue.h"
#include "message.h"
#include "response.h"
#include "fifo.h"
#include "task.h"
#include "factory.h"
#include "thread.h"

#if !defined(__CPU)
#define __CPU

namespace compute
{
	class cpu : public compute, public thread
	{
		::queue::queue<::compute::task> *queue;

		bool init;

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

#endif