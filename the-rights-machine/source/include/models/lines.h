#include "models/line.h"
#include "types/allocator.h"
#include "message/map.h"
#include "core/custom/pair.h"
#include "parser/json/legacy/json.h"

#if !defined(_MODELS_LINES)
#define _MODELS_LINES

namespace models
{
	namespace lines
	{
		template <long Y> class lines : public allocator::allocator<line::line, Y>, public json::request::json, public mapping::mapper
		{
			int index;

		public:
			line::line temp;

		public:
			lines() { index = 0; temp.parent(this); }
			lines(json *parent) { index = 0; json::parent(parent); }
			lines(lines<Y> const &source) { clear(); copy(source); }

		public:
			int identity() { return index; }

			bool flush() override
			{
				temp.lineID = index++;
				temp.itemID = progenitor();

				push(temp.lineID, temp.itemID);

				bool result = ::allocator::allocator<line::line, Y>::set(temp);
				
				temp.clear();

				return result;
			}

			void clear()
			{
				index = 0;

				temp.clear();
				mapper::empty();
				::allocator::allocator<line::line, Y>::reset();
			}

			void clear(bool parents)
			{
				index = 0;

				temp.clear();
				mapper::empty(parents);
				::allocator::allocator<line::line, Y>::reset();
			}

			string identifier() { return string("LINES"); }

			bool add(core::custom::pair source)
			{
				return temp.add(source);
			}

			void copy(lines<Y> const &source)
			{
				::allocator::allocator<line::line, Y>::copy(source);
				::mapping::mapper::copy(source);
			}

		public:
			lines<Y> operator=(const lines<Y>& source)
			{
				this->copy((lines<Y>&)source);
				return *this;
			}
		};
	};
};

#endif