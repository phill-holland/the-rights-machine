#include "compute.h"

#if !defined(__CPU)
#define __CPU

namespace compute
{
	class cpu : public compute
	{
	public:
		bool calculate(data::message::message *source, int length) 
		{
			return false;
		}
	};

	// has many worker threads for grid subtraction
};

#endif