#include "item.h"
#include "header.h"
#include "row.h"
#include "result.h"

#if !defined(__CPU_GRID)
#define __CPU_GRID

namespace compute
{
	namespace cpu
	{
		class grid
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

			void set(unsigned long x, unsigned long y)
			{
				if ((x >= width) || (y >= height)) return;
				data[(y * height) + x] = 1;
			}

			void minus(grid &right);
			void and(grid &right);
			bool compare(grid &right);

			bool push(row &source);

			void output();

		public:
			grid& operator-(const grid& source)
			{
				this->minus((grid&)source);
				return *this;
			}

			grid& operator&(const grid& source)
			{
				this->and((grid&)source);
				return *this;
			}

		protected:
			void makeNull();
			void cleanup();
		};
	};
};

#endif