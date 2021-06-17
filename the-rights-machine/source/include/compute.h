#include "queue.h"
#include "task.h"

#if !defined(__COMPUTE)
#define __COMPUTE

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