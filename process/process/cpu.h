#include "compute.h"
//#include "queue.h"
#include "message.h"
//#include "response.h"
#include "fifo.h"
#include "task.h"
#include "factory.h"

#if !defined(__CPU)
#define __CPU

namespace compute
{
	/*
	class m
	{
	public:
		data::message::message message;
		queue::queue<data::response> *response;

		// need copyt functions here, respoonse remains a ptr copy
		// message needs deep copy
	};
	*/
	class cpu : public compute
	{
		//queue::queue<m> *data;
		custom::fifo<::compute::task, 10L> queue;

	public:
		DWORD WINAPI background(thread *bt)
		{
			// check FIFO here
			return (DWORD)0;
		}

	public:
		cpu(queue::factory<data::message::message> *factory)
		{
		}
		/*
		bool calculate(data::message::message *source, int length) 
		{
			return false;
		}
		*/

		bool push(::compute::task &task)
		{
			// make copy of task.message..????
			// should happen by nature of adding to the queue? / rather avoid that on the way out though..??

			return queue.set(task);
		}
		/*
		bool push(m *source)
		{
			// need to copy the message

			return data->set(source);
		}
		*/
	};

	// has many worker threads for grid subtraction
};

#endif