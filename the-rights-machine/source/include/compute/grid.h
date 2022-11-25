#include "compute/interfaces/grid.h"
#include "compute/interfaces/row.h"
#include "compute/header.h"
#include "compute/row.h"
#include "compute/result.h"
#include "models/item.h"

#if !defined(_COMPUTE_CPU_GRID)
#define _COMPUTE_CPU_GRID

namespace compute
{
	namespace cpu
	{
		class grid : public ::compute::interfaces::grid
		{
		protected:
			const static unsigned long WIDTH = 255;
			const static unsigned long HEIGHT = 255;

		private:
			unsigned long width, height;
			unsigned long write_ptr;

			header **headers;
			int *data;

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