#include "component.h"
//#include "fifo.h"
#include "allocator.h"

#if !defined(__COMPONENTS)
#define __COMPONENTS

namespace data
{
	namespace components
	{
		/*
		class base : public json
		{
		public:
			base() { }
			base(json *parent) { clear(); json::parent(parent); }

			void clear() { }
			string identifier() { return string("COMPONENTS"); }
			bool add(custom::pair &source) { return false; }
		};
		*/
		template <long Y> class components : public allocator::allocator<component::component, Y>, public json
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
				bool result = ::allocator::allocator<component::component, Y>::set(temp);
				temp.clear();
				return result;
			}
			
			void clear() 
			{ 
				temp.clear();
				reset();
			} 

			string identifier() { return string("COMPONENTS"); }

			bool add(custom::pair &source)
			{
				return temp.add(source);
			}
		};
		/*
		template <long Y> class components : public custom::fifo<component::component, Y>, public base
		{
		public:
			void clear() override { this->empty(); }
		};
		*/
	};
};

#endif