#include "line.h"
#include "json.h"
#include "allocator.h"
#include "map.h"

#if !defined(__LINES)
#define __LINES

namespace data
{
	namespace lines
	{
		template <long Y> class lines : public allocator::allocator<line::line, Y>, public json, public mapping::mapper
		{
			int index;

		public:
			line::line temp;

		public:
			lines() { index = 0; temp.parent(this); }
			lines(json *parent) { index = 0; json::parent(parent); }

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

			string identifier() { return string("LINES"); }

			bool add(custom::pair &source)
			{
				return temp.add(source);
			}
		};
	};
};

#endif