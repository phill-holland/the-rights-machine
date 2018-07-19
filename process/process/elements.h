#include "element.h"
#include "allocator.h"
#include "string.h"
#include "map.h"

#if !defined(__ELEMENTS)
#define __ELEMENTS

namespace data
{
	namespace elements
	{
		template <long Y> class elements : public allocator::allocator<element::element, Y>, public json, public mapping::mapper
		{
			long index;

		public:
			element::element temp;

		public:
			elements() { index = 0L; temp.parent(this); }
			elements(json *parent) { index = 0L; json::parent(parent); }

		public:
			bool set(int parent)
			{
				temp.elementID = index++;
				temp.componentID = parent;
			}

			bool flush() override
			{				
				bool result = ::allocator::allocator<element::element, Y>::set(temp);
				
				push(temp.value);
				temp.clear();

				return result;
			}

			void clear() 
			{ 
				temp.clear();
				mapper::empty();
				reset();				
			}

			string identifier() { return string("ELEMENTS"); }

			bool add(custom::pair &source)
			{
				return temp.add(source);
			}
		};
	};
};

#endif