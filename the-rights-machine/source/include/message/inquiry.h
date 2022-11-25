#include "core/string/string.h"
#include "types/datetime.h"
#include "models/queries.h"
#include "models/items.h"
#include "models/lines.h"
//#include "models/components.h"
#include "models/elements.h"
#include "core/string/comparison.h"
//#include "common.h"
#include "database/interfaces/file.h"
#include <unordered_map>

#if !defined(__INQUIRY)
#define __INQUIRY

namespace data
{
	namespace message
	{
		//using namespace global;
		using namespace comparison;

		class inquiry : public json::request::json
		{
    		std::unordered_map<string, data::json::request::json *, hasher, equality> hash;
			std::unordered_map<string, queue::base *, hasher, equality> queue_hash;

			bool init;

		public:
			data::queries::queries<10L> queries;
		
		public:
			inquiry() { makeNull(); reset(); }
			inquiry(inquiry const &source) { makeNull(); reset(); copy(source); }
			~inquiry() { cleanup(); }

			bool initalised() { return init; }
			void reset();

			int identity() { return 0; }

			void clear();

			queue::base *findQ(string FQDN);
			data::json::request::json *find(string FQDN);

			void copy(inquiry const &source);

		public:
			string output();

		public:
			string identifier() { return string("INQUIRY"); }
			bool add(custom::pair source);

		public:
			inquiry& operator=(const inquiry& source)
			{
				this->copy((inquiry&)source);
				return *this;
			}

		protected:
			void makeNull();
			void cleanup();
		};
	};
};

#endif