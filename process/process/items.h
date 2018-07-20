#include "item.h"
#include "json.h"
#include "allocator.h"
#include "map.h"

#if !defined(__ITEMS)
#define __ITEMS

namespace data
{
	namespace items
	{
		template <long Y> class items : public allocator::allocator<item::item, Y>, public json, public mapping::mapper
		{
			long index;

		public:
			item::item temp;

		public:
			items() { index = 0L; temp.parent(this); }
			items(json *parent) { index = 0L; json::parent(parent); }

		public:
			bool set(int parent)
			{
				temp.itemID = index++;
				temp.messageID = parent;
			}

			bool flush() override
			{
				push(temp.name, temp.itemID, temp.messageID);

				bool result = ::allocator::allocator<item::item, Y>::set(temp);
				
				temp.clear();
				
				return result;
			}

			void clear()
			{
				index = 0L;
				temp.clear();
				mapper::empty();
				reset();
			}

			string identifier() { return string("ITEMS"); }

			bool add(custom::pair &source)
			{
				return temp.add(source);
			}
		};
	};
};

#endif