#include "queues/interfaces/queue.h"
#include "compute/task.h"

#if !defined(_COMPUTE_INTERFACES_COMPUTE)
#define _COMPUTE_INTERFACES_COMPUTE

namespace compute
{
	class compute : public ::queue::queue<::compute::task>
	{
	public:
		virtual bool get(::compute::task &destination) = 0;
		virtual bool set(::compute::task &source) = 0;

		virtual bool flush() = 0;
	};
};

#endif