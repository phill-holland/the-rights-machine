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
		//template <class X> 
		/*
		class item // public json::response::json
		{
		public:
			//X data;
			global::datetime created;

		public:
			virtual string identity() = 0;
		};

		template <class X> class chain //where X:item
		{
			std::vector<item<X>> data;
			// with a thread checking expiration dates

		public:
			DWORD WINAPI background(thread *bt);

		public:
			bool set(X &source) { return false; }

			X& get(unsigned long index)
			{
				return data[index];
			}

			item *allocate() = 0;

			long count() { return 0L; }
		};
		*/
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

		class responses : public thread, public custom::chain<::data::response::response>//, public data::json::response::json
		{
			static const unsigned long MAX = 255L;

		protected:
			DWORD WINAPI background(thread *bt);

		private:
			std::vector<data::response::response> data;
			std::unordered_map<string, unsigned long, hasher, equality> map;

			mutex::token token;

			bool init;

		public:
			responses() { makeNull(); reset(); }
			~responses() { cleanup(); }

			bool initalised() { return init; }
			void reset();

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


