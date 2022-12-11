#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include "cuda.cuh"
#include "row.cuh"
#include "models/item.h"
#include "compute/header.h"
#include "compute/result.h"

#if !defined(__CUDA_GRID)
#define __CUDA_GRID

namespace compute
{
	namespace gpu
	{
		class grid : public cuda::cuda
		{
		protected:
			static unsigned long GRIDS;
			static unsigned long THREADS;

			static unsigned long WIDTH;
			static unsigned long HEIGHT;

		private:
			unsigned long width, height;
			unsigned long write_ptr;

			header **headers;			
			int *data, *temp;

			bool init;

		public:
			grid(unsigned long width = WIDTH, unsigned long height = HEIGHT) { makeNull(); reset(width, height); }
			~grid() { cleanup(); }

			bool initalised() { return init; }
			void reset(unsigned long width, unsigned long height);

			void clear();

			bool isempty();

			void minus(grid &right);
			void AND(grid &right);
			bool compare(grid &right);

			bool push(::compute::interfaces::row *source);

			string output();

		public:
			grid& operator-(const grid& source)
			{
				this->minus((grid&)source);
				return *this;
			}

			grid& operator&(const grid& source)
			{
				this->AND((grid&)source);
				return *this;
			}

		protected:
			void makeNull();
			void cleanup();
		};
	};
};
 
#endif