#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include "cuda.cuh"
#include "compute/interfaces/row.h"
#include "compute/header.h"

#if !defined(__CUDA_ROW)
#define __CUDA_ROW

namespace compute
{
	namespace gpu
	{
		class row : public interfaces::row, public cuda::cuda
		{
		protected:
			static const unsigned long LENGTH = 255L;

		private:
			unsigned long length;

			header top;
			int *data;

			bool init;

		public:
			row(unsigned long length = LENGTH) { makeNull(); reset(length); }
			~row() { cleanup(); }

			bool initalised() { return init; }
			void reset(unsigned long length);

			void clear();

			int get(unsigned long idx);

			void set(header *source);
			bool set(unsigned long idx);

			int *raw() { return data; }

			bool copy(row const &source);

		public:
			unsigned long count() { return length; }
			bool first(header *destination) 
			{ 
				(*destination) = top; 
				return true; 
			}

		public:
			row& operator=(const row& source)
			{
				this->copy((row&)source);
				return *this;
			}

		protected:
			void makeNull();
			void cleanup();
		};
	};
};

#endif