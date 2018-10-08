#include "cuda_runtime.h"
#include "device_launch_parameters.h"

#if !defined(__CUDA)
#define __CUDA

namespace compute
{
	namespace cuda
	{
		class cuda
		{
			static long INSTANCES;

		public:
			cuda();
			~cuda();
		};
	};
};

#endif