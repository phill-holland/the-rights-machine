#include "string.h"
#include "datetime.h"
#include "queries.h"
#include "items.h"
#include "lines.h"
#include "components.h"
#include "elements.h"
#include "comparison.h"
#include "row.h"
#include <unordered_map>

#if !defined(__MESSAGE)
#define __MESSAGE

namespace data
{
	namespace message
	{
		using namespace global;
		using namespace comparison;

		class message : public json
		{
			std::unordered_map<string, data::json *, hasher, equality> hash;
			std::unordered_map<string, queue::base *, hasher, equality> queue_hash;

		public:
			int messageID;
			int userID;
			datetime created;
			datetime finished;

		private:
			bool init;

		public:
			data::queries::queries<10L> queries;
			data::items::items<10L> items;
			data::lines::lines<10L> lines;
			data::components::components<10L> components;
			data::elements::elements<10L> elements;

		public:
			message() { makeNull(); reset(); }
			message(message const &source) { makeNull(); reset(); copy(source); }
			~message() { cleanup(); }

			bool initalised() { return init; }
			void reset();

			int identity() { return messageID; }

			void clear();

			queue::base *findQ(string FQDN);
			data::json *find(string FQDN);

			void filter(compute::row **rows, unsigned long total, std::unordered_map<int, int> &map);

			void copy(message const &source);

		public:
			void output();

		public:
			string identifier() { return string("MESSAGE"); }
			bool add(custom::pair &source);

		public:
			message& operator=(const message& source)
			{
				this->copy((message&)source);
				return *this;
			}

		protected:
			void makeNull();
			void cleanup();
		};
	};
};

#endif