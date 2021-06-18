#include "custom/string.h"

#if !defined(__CHAIN)
#define __CHAIN

namespace custom
{
	template <class X> class chain
	{
	public:
		virtual unsigned long count() = 0;

//		virtual bool flush() = 0;

		virtual bool set(X &source) = 0;
		//virtual bool get(X &destination) = 0;

		virtual X get(unsigned long index) = 0;

		virtual X find(string &identity) = 0;

		virtual bool remove(string &identity) = 0;
		//virtual bool remove(unsigned long index) = 0;
	};
};

#endif