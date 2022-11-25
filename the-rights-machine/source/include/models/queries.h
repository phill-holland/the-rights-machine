#include "models/query.h"
#include "types/allocator.h"

#if !defined(__QUERIES)
#define __QUERIES

namespace data
{
	namespace queries
	{
		template <long Y> class queries : public allocator::allocator<query::query, Y>, public json::request::json, public mapping::mapper
		{
			int index;

		public:
			query::query temp;

		public:
			queries() { index = 0; temp.parent(this); }
			queries(json *parent) { index = 0; json::parent(parent); }
			queries(queries<Y> const &source) { clear(); copy(source); }

		public:
			int identity() { return index; }

			bool flush() override
			{
				temp.queryID = index++;
				temp.messageID = progenitor();

				push(temp.queryID, temp.messageID);

				bool result = ::allocator::allocator<query::query, Y>::set(temp);

				temp.clear();

				return result;
			}

			void clear()
			{
				index = 0;

				temp.clear();
				mapper::empty();
				::allocator::allocator<query::query, Y>::reset();
			}

			string identifier() { return string("QUERIES"); }

			bool add(custom::pair source)
			{
				return temp.add(source);
			}

			void copy(queries<Y> const &source)
			{
				::allocator::allocator<query::query, Y>::copy(source);
				::mapping::mapper::copy(source);
			}

		public:
			queries<Y> operator=(const queries<Y>& source)
			{
				this->copy((queries<Y>&)source);
				return *this;
			}
		};
	};
};

#endif