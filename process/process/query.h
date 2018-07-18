#include "string.h"
#include "datetime.h"
#include "json.h"
#include "zone.h"

#if !defined(__QUERY)
#define __QUERY

using namespace global;

namespace data
{
	namespace query
	{
		class query : public json, public zone::zone
		{
		public:
			int queryID;
			int messageID;
			//datetime start;
			//datetime end;

		public:
			query() { clear(); }
			query(json *parent) { clear(); json::parent(parent); }
			query(query const &source) { clear(); copy(source); }

			void clear();

			//query spawn(datetime &start, datetime &end);

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
		};
	};
};

#endif