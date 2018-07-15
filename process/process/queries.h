#include "query.h"
#include "allocator.h"

#if !defined(__QUERIES)
#define __QUERIES

namespace data
{
	namespace queries
	{
		template <long Y> class queries : public allocator::allocator<query::query, Y>, public json
		{
			long index;

		public:
			query::query temp;

		public:
			queries() { index = 0L; temp.parent(this); }
			queries(json *parent) { index = 0L; json::parent(parent); }

		public:
			bool set(int parent)
			{
				temp.queryID = index++;
				temp.messageID = parent;
			}

			bool flush() override
			{
				bool result = ::allocator::allocator<query::query, Y>::set(temp);
				temp.clear();
				return result;
			}

			void clear()
			{
				temp.clear();
				reset();
			}

			string identifier() { return string("QUERIES"); }

			bool add(custom::pair &source)
			{
				return temp.add(source);
			}
		};
	};
};

#endif