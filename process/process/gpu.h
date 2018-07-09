#include "compute.h"

#if !defined(__GPU)
#define __GPU

namespace compute
{
	class gpu : public compute
	{
	public:
		bool calculate(data::message::message *source, int length)
		{
			return false;
		}
	};
};

#endif