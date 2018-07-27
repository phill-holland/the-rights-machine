#include "response.h"
#include <vector>

#if !defined(__RESPONSES)
#define __RESPONSES

namespace data
{
	namespace response
	{
		class responses
		{
			static const unsigned long MAX = 255L;

		private:
			std::vector<data::response::response> data;

			bool init;

		public:
			responses() { makeNull(); reset(); }
			~responses() { cleanup(); }

			bool initalised() { return init; }
			void reset();

			void clear() { return data.clear(); }

			bool add(data::response::response &source)
			{
				if (count() >= MAX) return false;

				data.push_back(source);

				return true;
			}

			data::response::response& get(unsigned long index)
			{
				return data[index];
			}

			long count() { return (unsigned long)data.size(); }

		public:
			data::response::response& operator[](int index)
			{
				return get((unsigned long)index);
			}

		protected:
			void makeNull() { }
			void cleanup() { }
		};
	};
};

#endif


