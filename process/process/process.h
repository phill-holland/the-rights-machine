#include "compute.h"
#include "databases.h"
#include "queue.h"
#include "item.h"

#if !defined(__PROCESS)
#define __PROCESS

namespace process
{
	//template <database::connection X, compute::compute Y> class process
	class process
	{
		queue::queue<data::item::item> *queue; // pass in constructor
		compute::compute *compute; // pass into constructor

	public:
	};
};

#endif