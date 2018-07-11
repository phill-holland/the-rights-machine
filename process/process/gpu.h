#include "compute.h"
#include "task.h"

#if !defined(__GPU)
#define __GPU

namespace compute
{
	class gpu : public compute
	{
	public:
		bool push(::compute::task &task) { return false; }
	};
};

#endif