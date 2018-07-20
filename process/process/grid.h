#include "item.h"
#include "row.h"

#if !defined(__GRID)
#define __GRID

namespace compute
{
	class grid
	{
	protected:
		const static unsigned long WIDTH = 255;
		const static unsigned long HEIGHT = 255;

	private:
		unsigned long width, height;

		bool init;

		int *data;

	public:
		grid(unsigned long width = WIDTH, unsigned long height = HEIGHT) { makeNull(); reset(width, height); }
		~grid() { cleanup(); }

		bool initalised() { return init; }
		void reset(unsigned long width, unsigned long height);

		void clear();

		void set(unsigned long x, unsigned long y)
		{
			if ((x >= width) || (y >= height)) return;
			data[(y * height) + x] = 1;
		}

		void minus(grid &right);
	
		void push(row &source);

	protected:
		void makeNull();
		void cleanup();
	};

	// need two of these, the have, and have not's for subtraction

	// grid of components
	// the compute engine storage
	// should be an abstract class
	// components must be hash table mapped
	// and item they belong to tracked

	// in cpu land, parallel add
	// in gpu land, memory is GPU memory
	// gpu -- may not need to copy data into system
	// just call "set" function to populate -- on/off
	/*
	enum type { CPU = 0, GPU = 1 };

	template <type X> class grid
	{
	public:
		virtual bool insert(long index, data::item::item &item) = 0;
		virtual bool fetch(long index, data::item::item &item) = 0;

	};*/
};

#endif