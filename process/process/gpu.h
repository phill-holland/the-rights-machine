#include "compute.h"
#include "task.h"
#include "factory.h"

#if !defined(__GPU)
#define __GPU

namespace compute
{
	class gpu : public compute
	{
	public:
		gpu(queue::factory<data::message::message> *factory)
		{
		}

		bool push(::compute::task &task) { return false; }
	};
};

#endif