#include "message.h"

#if !defined(__COMPUTE)
#define __COMPUTE

namespace compute
{
	class compute
	{
	public:
		virtual bool calculate(data::message *source, int length) = 0;
	};
};

#endif