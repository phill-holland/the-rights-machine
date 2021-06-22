#include "custom/string.h"
#include "datetime.h"
#include "queries.h"
#include "items.h"
#include "lines.h"
#include "components.h"
#include "elements.h"
#include "comparison.h"
#include "common.h"
#include "file.h"
#include <unordered_map>

#if !defined(__MESSAGE)
#define __MESSAGE

namespace data
{
	namespace message
	{
		using namespace global;
		using namespace comparison;

		class message : public json::request::json
		{
			std::unordered_map<string, data::json::request::json *, hasher, equality> hash;
			std::unordered_map<string, queue::base *, hasher, equality> queue_hash;

		public:
			int messageID;
			//string user; // call different name?
			//string apikey;
			string guid;
			datetime created;
			datetime finished;

		private:
			bool init;

		public:
			data::queries::queries<10L> queries;
			data::items::items<10L> items;
			data::lines::lines<10L> lines;
			data::components::line::components<10L> components;
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
			data::json::request::json *find(string FQDN);

			void filter(compute::common::row **rows, unsigned long total, std::unordered_map<int, int> &map);

			bool load(file::file<data::item::item> *source);
			bool load(file::file<data::query::query> *source);
			bool load(file::file<data::line::line> *source);
			bool load(file::file<data::component::line::component> *source);
			bool load(file::file<data::element::element> *source);

			bool save(file::file<data::item::item> *destination);
			bool save(file::file<data::query::query> *destination);
			bool save(file::file<data::line::line> *destination);
			bool save(file::file<data::component::line::component> *destination);
			bool save(file::file<data::element::element> *destination);

			void copy(message const &source);

		public:
			void output();

		public:
			string identifier() { return string("MESSAGE"); }
			bool add(custom::pair source);

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