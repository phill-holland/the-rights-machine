#include "core/string/string.h"
#include "core/custom/datetime.h"
#include "types/zone.h"
#include "core/custom/pair.h"
#include "parser/json/legacy/json.h"
#include "models/components/query/components.h"
#include "models/elements.h"
#include "database/interfaces/file.h"
#include "compute/interfaces/row.h"

#if !defined(__QUERY)
#define __QUERY

namespace models
{
	namespace query
	{
		class query : public json::request::json , public types::zone
		{
			bool init;

		public:
			int queryID;
			int messageID;

		public:
			models::components::query::components<10L> components;
			models::elements::elements<10L> elements;

		public:
			query() { makeNull(); reset(); }
			query(json *parent) { makeNull(); json::parent(parent); reset(); }
			query(query const &source) { makeNull(); clear(); copy(source); }
			~query() { cleanup(); }

			bool initalised() { return init; }
			void reset();

			int identity() { return queryID; }

			void clear();

			void filter(compute::interfaces::row **rows, unsigned long total, unsigned long lines);
			
			bool load(file::file<models::component::query::component> *source);
			bool load(file::file<models::element::element> *source);

			bool save(file::file<models::component::query::component> *destination);
			bool save(file::file<models::element::element> *destination);

			void copy(query const &source);

		public:
			string output();

		public:
			string identifier() { return string("QUERY"); }
			bool add(core::custom::pair source);

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