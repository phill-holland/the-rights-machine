#include "element.h"
//#include "fifo.h"
#include "allocator.h"

#if !defined(__ELEMENTS)
#define __ELEMENTS

namespace data
{
	namespace elements
	{
		/*
		class base : public json
		{
		public:
			base() { }
			base(json *parent) { clear(); json::parent(parent); }

			void clear() { }
			string identifier() { return string("ELEMENTS"); }
			bool add(custom::pair &source) { return false; }
		};
		*/
		template <long Y> class elements : public allocator::allocator<element::element, Y>, public json
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
				temp.clear();
				return result;
			}

			void clear() 
			{ 
				temp.clear();
				reset();
			}

			string identifier() { return string("ELEMENTS"); }

			bool add(custom::pair &source)
			{
				return temp.add(source);
			}
		};
		/*
		template <long Y> class elements : public custom::fifo<element::element, Y>, public base
		{
		public:
			void clear() override { this->empty(); }
		};
		*/
	};
};

#endif