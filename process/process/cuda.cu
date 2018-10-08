#include "cuda.cuh"

long compute::cuda::cuda::INSTANCES = 0L;

compute::cuda::cuda::cuda()
{
	if(INSTANCES == 0L) cudaSetDevice(0);
}

compute::cuda::cuda::~cuda()
{
	if (--INSTANCES == 0L) cudaDeviceReset();
}