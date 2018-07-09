#include "item.h"
//#include "fifo.h"
#include "json.h"
#include "allocator.h"

#if !defined(__ITEMS)
#define __ITEMS

namespace data
{
	namespace items
	{
		/*
		class base : public json
		{
		public:
			base() { }
			base(json *parent) { clear(); json::parent(parent); }

			virtual void clear() { }
			string identifier() { return string("ITEMS"); }
			bool add(custom::pair &source) { return false; }
		};
		*/
		template <long Y> class items : public allocator::allocator<item::item, Y>, public json
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
				bool result = ::allocator::allocator<item::item, Y>::set(temp);
				temp.clear();
				return result;
			}

			void clear()
			{
				temp.clear();
				reset();
			}

			string identifier() { return string("ITEMS"); }

			bool add(custom::pair &source)
			{
				return temp.add(source);
			}
		};
		//template <long Y> class items : public allocator::allocator<::data::item::item, Y>//, public base
		//{
		//public:
			//void clear() override { this->reset(); }
		//};
		/*
		template <long Y> class items : public custom::fifo<::data::item::item, Y>, public base
		{
		public:
			void clear() override { this->empty(); }
		};
		*/
	};
};

#endif