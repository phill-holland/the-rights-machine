#include "queue.h"
#include "message.h"
#include "response.h"
#include "chain.h"

#if !defined(__TASK)
#define __TASK

namespace compute
{
	class task
	{
	public:
		data::message::message message;
		custom::chain<data::response::response> *response;
	};
};

#endif