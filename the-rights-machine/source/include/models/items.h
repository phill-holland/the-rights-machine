#include "item.h"
//#include "json.h"
#include "types/allocator.h"
#include "message/map.h"
#include "database/interfaces/file.h"

#if !defined(_MODELS_ITEMS_ITEMS)
#define _MODEL_ITEMS_ITEMS

namespace models
{
	namespace items
	{
		template <long Y> class items : public allocator::allocator<item::item, Y>/*, public json::request::json*/, public mapping::mapper
		{
			int index;

		public:
			item::item temp;

		public:
			items() { index = 0; temp.parent(this); }
			items(json *parent) { index = 0; json::parent(parent); }
			items(items<Y> const &source) { clear(); copy(source); }

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

/*
			string identifier() { return string("ITEMS"); }

			bool add(custom::pair source)
			{
				return temp.add(source);
			}
*/
			void copy(items<Y> const &source)
			{
				::allocator::allocator<item::item, Y>::copy(source);
				::mapping::mapper::copy(source);
			}

		public:
			items<Y> operator=(const items<Y>& source)
			{
				this->copy((items<Y>&)source);
				return *this;
			}
		};
	};
};

#endif