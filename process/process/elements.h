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
			int index;

		public:
			element::element temp;

		public:
			elements() { index = 0; temp.parent(this); }
			elements(json *parent) { index = 0; json::parent(parent); }

		public:
			int identity() { return index; }

			bool flush() override
			{	
				temp.elementID = index++;
				temp.componentID = progenitor();

				push(temp.value, temp.elementID, temp.componentID);

				bool result = ::allocator::allocator<element::element, Y>::set(temp);
								
				temp.clear();

				return result;
			}

			void clear() 
			{ 
				index = 0;
				temp.clear();
				mapper::empty();
				::allocator::allocator<element::element, Y>::reset();
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