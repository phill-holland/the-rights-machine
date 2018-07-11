#include "queue.h"

#if !defined(__FACTORY)
#define __FACTORY

namespace queue
{
	template <class X> class factory
	{
	public:
		virtual queue<X> *get() = 0;
	};
};

#endif