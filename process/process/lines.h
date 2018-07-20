#include "line.h"
#include "json.h"
#include "allocator.h"

#if !defined(__LINES)
#define __LINES

namespace data
{
	namespace lines
	{
		template <long Y> class lines : public allocator::allocator<line::line, Y>, public json
		{
			long index;

		public:
			line::line temp;

		public:
			lines() { index = 0L; temp.parent(this); }
			lines(json *parent) { index = 0L; json::parent(parent); }

		public:
			bool set(int parent)
			{
				temp.lineID = index++;
				temp.itemID = parent;
			}

			bool flush() override
			{
				bool result = ::allocator::allocator<line::line, Y>::set(temp);
				temp.clear();
				return result;
			}

			void clear()
			{
				index = 0L;

				temp.clear();
				reset();
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