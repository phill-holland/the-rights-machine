#include "queue.h"
#include "message.h"
#include "response.h"

#if !defined(__TASK)
#define __TASK

namespace compute
{
	class task
	{
	public:
		data::message::message message;
		queue::queue<data::response> *response;

		// need copyt functions here, respoonse remains a ptr copy
		// message needs deep copy

	//public:
		//task(data::message::message *msg = NULL, queue::queue<data::response> *rsp = NULL)
		//{
			//message = msg;
			//response = rsp;
		//}
	};
};

#endif