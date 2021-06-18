#include "response.h"
#include "json.h"
#include "thread.h"
#include "chain.h"
#include "comparison.h"
#include <vector>
#include <unordered_map>

#if !defined(__RESPONSES)
#define __RESPONSES

namespace data
{
	namespace response
	{
		// ****

		// this class also needs to be a thread
		// to check expiration dates

		// response class, need to add many children
		// add error class to response
		// turn this class into allocator
		// inherit from response::json
		// add find function
		// overload set function

		using namespace comparison;

		class responses : public thread, public custom::chain<::data::response::response>
		{
			static const unsigned long MAX = 255L;

		protected:
			void background(thread *bt);

		private:
			data::response::response **data;
			std::unordered_map<string, unsigned long, hasher, equality> map;

			mutex::token token;

			unsigned long total;
			unsigned long length;

			bool init;

		public:
			responses(unsigned long total = MAX) { makeNull(); reset(total); }
			~responses() { cleanup(); }

			bool initalised() { return init; }
			void reset(unsigned long total);

			void clear();// { return data.clear(); }

			bool set(data::response::response &source);
			/*{
				if (count() >= MAX) return false;

				data.push_back(source);

				return true;
			}*/

			data::response::response get(unsigned long index);
			/*{
				return data[index];
			}*/

			unsigned long count();// { return (unsigned long)data.size(); }

			//string identifier() { return string("response"); }

			//unsigned long pairs() { return 0UL; }
			//custom::pair& pull(unsigned long index) { return custom::pair(); }

			//virtual bool set(X &source) = 0;
			//virtual bool get(X &destination) = 0;

			//virtual X& get(unsigned long index) = 0;

			data::response::response find(string &identity);

			bool remove(string &identity);

		public:
			data::response::response operator[](int index)
			{
				return get((unsigned long)index);
			}

		protected:
			void makeNull();
			void cleanup();
		};
	};
};

#endif


