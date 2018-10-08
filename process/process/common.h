#include "header.h"

#if !defined(__COMMON)
#define __COMMON

namespace compute
{
	namespace common
	{
		class row
		{
		public:
			virtual void clear() = 0;

			virtual int get(unsigned long idx) = 0;

			virtual void set(header &source) = 0;
			virtual bool set(unsigned long idx) = 0;

			virtual int *raw() = 0;

			virtual header first() = 0;

			virtual unsigned long count() = 0;
		};

		class grid
		{
		public:
			virtual bool push(row *source) = 0;
		};
	};
};

#endif