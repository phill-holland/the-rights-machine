#include "compute.h"
#include "database.h"

#if !defined(__PROCESS)
#define __PROCESS

namespace process
{
	template <database::connection X, compute::compute Y> class process
	{
	public:
	};
};

#endif