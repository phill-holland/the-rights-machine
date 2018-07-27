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
		template <long Y> class items : public allocator::allocator<item::item, Y>, public json::request::json, public mapping::mapper
		{
			int index;

		public:
			item::item temp;

		public:
			items() { index = 0; temp.parent(this); }
			items(json *parent) { index = 0; json::parent(parent); }

		public:
			int identity() { return index; }

			bool flush() override
			{
				temp.itemID = index++;
				temp.messageID = progenitor();

				push(temp.name, temp.itemID, temp.messageID);

				bool result = ::allocator::allocator<item::item, Y>::set(temp);
				
				temp.clear();
				
				return result;
			}

			void clear()
			{
				index = 0;
				temp.clear();
				mapper::empty();
				::allocator::allocator<item::item, Y>::reset();
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