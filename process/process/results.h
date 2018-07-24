#include "result.h"

# if !defined(__RESULTS)
#define __RESULTS

namespace compute
{
	template <long Y> class results : public allocator::allocator<compute::result, Y>
	{
	};
};

#endif