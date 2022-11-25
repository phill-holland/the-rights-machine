#include "element.h"
#include "types/allocator.h"
#include "core/string/string.h"
#include "message/map.h"
#include "core/custom/pair.h"
#include "parser/json/json.h"

#if !defined(_MODELS_ELEMENTS)
#define _MODELS_ELEMENTS

namespace models
{
	namespace elements
	{
		template <long Y> class elements : public allocator::allocator<element::element, Y>, public json::request::json, public mapping::mapper
		{
			int index;

		public:
			element::element temp;

		public:
			elements() { index = 0; temp.parent(this); }
			elements(json *parent) { index = 0; json::parent(parent); }
			elements(elements<Y> const &source) { clear(); copy(source); }

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

			bool add(core::custom::pair source)
			{
				return temp.add(source);
			}

			void copy(elements<Y> const &source)
			{
				::allocator::allocator<element::element, Y>::copy(source);
				::mapping::mapper::copy(source);
			}

		public:
			elements<Y> operator=(const elements<Y>& source)
			{
				this->copy((elements<Y>&)source);
				return *this;
			}
		};
	};
};

#endif