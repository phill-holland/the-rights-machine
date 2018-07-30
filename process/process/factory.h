#include "queue.h"

#include "chain.h"

#if !defined(__FACTORY)
#define __FACTORY

namespace queue
{
	template <class X> class factory
	{
	public:
		virtual queue<X> *get() = 0;
	};

	template <class X> class chain_factory
	{
	public:
		virtual custom::chain<X> *get() = 0;
	};
};

#endif