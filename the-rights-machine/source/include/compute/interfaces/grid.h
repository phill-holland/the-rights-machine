#include "row.h"

#if !defined(_COMPUTE_INTERFACES_GRID)
#define _COMPUTE_INTERFACES_GRID

namespace compute
{
	namespace interfaces
	{
		class grid
		{
		public:
			virtual bool push(row *source) = 0;
		};
	};
};

#endif