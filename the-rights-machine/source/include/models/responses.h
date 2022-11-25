#include "interfaces/chain.h"
#include "models/response.h"
#include "core/string/comparison.h"
#include "core/custom/pair.h"
#include "parser/json/json.h"
#include <vector>
#include <unordered_map>

#if !defined(__RESPONSES)
#define __RESPONSES

namespace data
{
	namespace response
	{
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

			void clear();

			bool set(data::response::response &source);
			data::response::response get(unsigned long index);

			unsigned long count();

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


