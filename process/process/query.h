#include "string.h"
#include "datetime.h"
#include "json.h"
#include "zone.h"
#include "components.h"
#include "elements.h"
#include "file.h"
#include "common.h"

#if !defined(__QUERY)
#define __QUERY

using namespace global;

namespace data
{
	namespace query
	{
		class query : public json::request::json , public zone::zone
		{
			bool init;

		public:
			int queryID;
			int messageID;

		public:
			data::components::query::components<10L> components;
			data::elements::elements<10L> elements;

		public:
			query() { makeNull(); reset(); }
			query(json *parent) { makeNull(); json::parent(parent); reset(); }
			query(query const &source) { makeNull(); clear(); copy(source); }
			~query() { cleanup(); }

			bool initalised() { return init; }
			void reset();

			int identity() { return queryID; }

			void clear();

			void filter(compute::common::row **rows, unsigned long total, unsigned long lines);
			
			bool load(file::file<data::component::query::component> *source);
			bool load(file::file<data::element::element> *source);

			bool save(file::file<data::component::query::component> *destination);
			bool save(file::file<data::element::element> *destination);

			void copy(query const &source);

		public:
			void output();

		public:
			string identifier() { return string("QUERY"); }
			bool add(custom::pair &source);

		public:
			query& operator=(const query& source)
			{
				this->copy((query&)source);
				return *this;
			}

		protected:
			void makeNull();
			void cleanup();
		};
	};
};

#endif