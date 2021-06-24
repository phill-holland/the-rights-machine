#include "queue.h"
#include "message.h"
#include "response.h"
#include "chain.h"
#include "notification.h"

#if !defined(__TASK)
#define __TASK

namespace compute
{
	class task
	{
	public:
		data::message::message message;
		custom::chain<data::response::response> *response;
		notification::notification *notify;

	public:
		task() 
		{
			response = NULL;
			notify = NULL;
		}
	};
};

#endif