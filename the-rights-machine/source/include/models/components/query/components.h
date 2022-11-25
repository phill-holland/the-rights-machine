#include "models/component/query/component.h"
#include "types/allocator.h"
#include "message/map.h"
#include "core/custom/pair.h"
#include "parser/json/json.h"

#if !defined(_MODELS_COMPONENTS_QUERY_COMPONENT)
#define _MODELS_COMPONENTS_QUERY_COMPONENT

namespace models
{
	namespace components
	{		
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

				bool add(core::custom::pair source)
				{
					return temp.add(source);
				}

				void copy(components<Y> const &source)
				{
					::allocator::allocator<component::query::component, Y>::copy(source);
					::mapping::mapper::copy(source);
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