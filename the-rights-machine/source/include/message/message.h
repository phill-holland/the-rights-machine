#include "database/interfaces/file.h"
#include "core/string/string.h"
#include "core/string/comparison.h"
#include "types/datetime.h"
#include "models/queries.h"
#include "models/items.h"
#include "models/lines.h"
#include "models/components/line/components.h"
#include "models/elements.h"
#include "message/inquiry.h"
#include "core/custom/pair.h"
#include "parser/json/json.h"
#include <unordered_map>

#if !defined(__MESSAGE)
#define __MESSAGE

namespace data
{
	namespace message
	{
		using namespace comparison;

		class mapping
		{
		public:
			class map
			{
			public:
				int index;
				int lineID;

			public:
				map(int _index = 0, int _lineID = 0) 
				{ 
					index = _index; 
					lineID = _lineID; 
				}
			};

		public:
			std::unordered_map<int, map> in;
			std::unordered_map<int, map> out;

		public:
			mapping() { clear(); }

			void clear() { in.clear(); out.clear(); }

			bool hasIn() { return in.size() > 0; }
			bool hasOut() { return out.size() > 0; }
		};

		class message : public json::request::json
		{
			std::unordered_map<string, ::json::request::json *, hasher, equality> hash;
			std::unordered_map<string, queue::base *, hasher, equality> queue_hash;

		public:
			int messageID;
			string name;
			string guid;
			datetime created;
			datetime finished;

		private:
			bool init;

		public:
			models::items::items<10L> items;
			models::lines::lines<10L> lines;
			models::components::line::components<10L> components;
			models::elements::elements<10L> elements;

		public:
			message() { makeNull(); reset(); }
			message(message const &source) { makeNull(); reset(); copy(source); }
			~message() { cleanup(); }

			bool initalised() { return init; }
			void reset();

			int identity() { return messageID; }

			void clear();

			queue::base *findQ(string FQDN);
			::json::request::json *find(string FQDN);

			message split(inquiry &inquiry, mapping &destination);

			void filter(compute::interfaces::row **rows, unsigned long total, 
						std::unordered_map<int, mapping::map> &map);

			bool load(file::file<models::item::item> *source);
			//bool load(file::file<data::query::query> *source);
			bool load(file::file<models::line::line> *source);
			bool load(file::file<models::component::line::component> *source);
			bool load(file::file<models::element::element> *source);

			bool save(file::file<models::item::item> *destination);
			//bool save(file::file<data::query::query> *destination);
			bool save(file::file<models::line::line> *destination);
			bool save(file::file<models::component::line::component> *destination);
			bool save(file::file<models::element::element> *destination);

			void copy(message const &source);

		public:
			string output();

		public:
			string identifier() { return string("MESSAGE"); }
			bool add(core::custom::pair source);

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