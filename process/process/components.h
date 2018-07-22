#include "component.h"
#include "allocator.h"
#include "map.h"

#if !defined(__COMPONENTS)
#define __COMPONENTS

namespace data
{
	namespace components
	{
		template <long Y> class components : public allocator::allocator<component::component, Y>, public json, public mapping::mapper
		{
			long index;

		public:
			component::component temp;

		public:
			components() { index = 0L; temp.parent(this); }
			components(json *parent) { index = 0L; json::parent(parent); }

		public:
			bool set(int parent)
			{
				temp.componentID = index++;
				temp.lineID = parent;				
			}

			bool flush() override
			{
				push(temp.name, temp.componentID, temp.lineID);

				bool result = ::allocator::allocator<component::component, Y>::set(temp);
				
				temp.clear();
				
				return result;
			}
			
			void clear() 
			{ 
				index = 0L;
				temp.clear();
				mapper::empty();
				::allocator::allocator<component::component, Y>::reset();
			} 

			string identifier() { return string("COMPONENTS"); }

			bool add(custom::pair &source)
			{
				return temp.add(source);
			}
		};
	};
};

#endif