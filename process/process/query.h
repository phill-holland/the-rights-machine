#include "string.h"
#include "datetime.h"
#include "json.h"
#include "zone.h"
#include "components.h"
#include "elements.h"

#if !defined(__QUERY)
#define __QUERY

using namespace global;

namespace data
{
	namespace query
	{
		class query : public json, public zone::zone
		{
			bool init;

		public:
			int queryID;
			int messageID;

		public:
			data::components::components<10L> components;
			data::elements::elements<10L> elements;

		public:
			query() { makeNull(); reset(); }
			query(json *parent) { makeNull(); json::parent(parent); reset(); }
			query(query const &source) { makeNull(); clear(); copy(source); }
			~query() { cleanup(); }

			bool initalised() { return init; }
			void reset();

			void clear();

			void copy(query const &source);

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