#include "item.h"

#if !defined(__GRID)
#define __GRID

namespace grid
{
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
	enum type { CPU = 0, GPU = 1 };

	template <type X> class grid
	{
	public:
		virtual bool insert(long index, data::item::item &item) = 0;
		virtual bool fetch(long index, data::item::item &item) = 0;

		virtual bool subtract(grid<X> &right) = 0;
	};
};

#endif