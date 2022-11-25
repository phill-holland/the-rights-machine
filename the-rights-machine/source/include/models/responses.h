#include "interfaces/chain.h"
#include "models/response.h"
#include "core/string/comparison.h"
#include "core/custom/pair.h"
#include "core/threading/thread.h"
#include "core/threading/mutex.h"
#include "parser/json/json.h"
#include <vector>
#include <unordered_map>

#if !defined(__RESPONSES)
#define __RESPONSES

namespace models
{
	namespace response
	{
		using namespace comparison;

		class responses : public core::threading::thread, public custom::chain<::models::response::response>
		{
			static const unsigned long MAX = 255L;

		protected:
			void background(thread *bt);

		private:
			models::response::response **data;
			std::unordered_map<string, unsigned long, hasher, equality> map;

			core::threading::mutex::token token;

			unsigned long total;
			unsigned long length;

			bool init;

		public:
			responses(unsigned long total = MAX) { makeNull(); reset(total); }
			~responses() { cleanup(); }

			bool initalised() { return init; }
			void reset(unsigned long total);

			void clear();

			bool set(models::response::response &source);
			models::response::response get(unsigned long index);

			unsigned long count();

			models::response::response find(string &identity);

			bool remove(string &identity);

		public:
			models::response::response operator[](int index)
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


