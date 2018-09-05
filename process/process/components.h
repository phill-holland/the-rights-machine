#include "component.h"
#include "allocator.h"
#include "map.h"
#include "log.h"

#if !defined(__COMPONENTS)
#define __COMPONENTS

namespace data
{
	namespace components
	{
		namespace line
		{
			template <long Y> class components : public allocator::allocator<component::line::component, Y>, public json::request::json, public mapping::mapper
			{
				int index;

			public:
				component::line::component temp;

			public:
				components() { index = 0; temp.parent(this); }
				components(json *parent) { index = 0; json::parent(parent); }

			public:
				int identity() { return index; }

				bool flush() override
				{
					temp.componentID = index++;
					temp.lineID = progenitor();

					push(temp.name, temp.componentID, temp.lineID);

					bool result = ::allocator::allocator<component::line::component, Y>::set(temp);

					temp.clear();

					return result;
				}

				void clear()
				{
					index = 0;
					temp.clear();
					mapper::empty();
					::allocator::allocator<component::line::component, Y>::reset();
				}

				string identifier() { return string("COMPONENTS"); }

				bool add(custom::pair &source)
				{
					return temp.add(source);
				}
			};
		};

		namespace query
		{
			template <long Y> class components : public allocator::allocator<component::query::component, Y>, public json::request::json, public mapping::mapper
			{
				int index;

			public:
				component::query::component temp;

			public:
				components() { index = 0; temp.parent(this); }
				components(json *parent) { index = 0; json::parent(parent); }
				components(components<Y> const &source) { clear(); copy(source); }

			public:
				int identity() { return index; }

				bool flush() override
				{
					temp.componentID = index++;
					temp.queryID = progenitor();

					push(temp.name, temp.componentID, temp.queryID);

					bool result = ::allocator::allocator<component::query::component, Y>::set(temp);

					temp.clear();

					return result;
				}

				void clear()
				{
					index = 0;
					temp.clear();
					mapper::empty();
					::allocator::allocator<component::query::component, Y>::reset();
				}

				string identifier() { return string("COMPONENTS"); }

				bool add(custom::pair &source)
				{
					return temp.add(source);
				}

				void copy(components<Y> const &source)
				{
					::allocator::allocator<component::query::component, Y>::copy(source);
				}

			public:
				components<Y> operator=(const components<Y>& source)
				{
					this->copy((components<Y>&)source);
					return *this;
				}
			};
		};
	};
};

#endif