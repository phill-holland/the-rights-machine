#include "record.h"
#include "datetime.h"
#include "message.h"
#include "item.h"
#include "line.h"
#include "component.h"
#include "request.h"
#include "response.h"
#include "guid.h"

#if !defined(__RECORDS)
#define __RECORDS

#warning split into seperate files

namespace database
{
	namespace records
	{
		using namespace global;

		class message : public record::record<data::message::message>
		{
		public:
			guid::guid messageID;
			guid::guid user;
			guid::guid apikey;
			guid::guid guid;
			guid::guid tag;
			TIMESTAMP_STRUCT created;
			
		public:
			message() { clear(); }
			message(const message &source) { }

			void clear();

			bool bind(database::recordset *recordset);
			void set(data::message::message &source);
		};

		class item : public record::record<data::item::item>
		{
		public:
			static const long MAX = 128L;

		public:
			guid::guid itemID;
			guid::guid messageID;
			char name[MAX];

		public:
			item() { clear(); }
			item(const item &source) { }
			
			void clear();

			bool bind(database::recordset *recordset);
			void set(data::item::item &source);
		};

		class query : public record::record<data::query::query>
		{
		public:
			guid::guid queryID;
			guid::guid messageID;

		public:
			query() { clear(); }
			query(const query &source) { }

			void clear();

			bool bind(database::recordset * recordset);
			void set(data::query::query &source);
		};

		class line : public record::record<data::line::line>
		{
		public:
			guid::guid lineID;
			guid::guid itemID;
			TIMESTAMP_STRUCT start;
			TIMESTAMP_STRUCT end;
			long exclusivityID;
			long typeID;

		public:
			line() { clear(); }
			line(const line &source) { }

			void clear();

			bool bind(database::recordset *recordset);
			void set(data::line::line &source);
		};

		namespace component
		{
			namespace line
			{
				class component : public record::record<data::component::line::component>
				{
				public:
					static const long MAX = 128L;
					static const long TYPE = 2L;

				public:
					guid::guid componentID;
					guid::guid lineID;
					long type;
					char name[MAX];

				public:
					component() { clear(); }
					component(const component &source) { }

					void clear();

					bool bind(database::recordset *recordset);
					void set(data::component::line::component &source);
				};
			};

			namespace query
			{
				class component : public record::record<data::component::query::component>
				{
				public:
					static const long MAX = 128L;
					static const long TYPE = 2L;

				public:
					guid::guid componentID;
					guid::guid queryID;
					long type;
					char name[MAX];

				public:
					component() { clear(); }
					component(const component &source) { }

					void clear();

					bool bind(database::recordset *recordset);
					void set(data::component::query::component &source);
				};
			};
		};

		class element : public record::record<data::element::element>
		{
		public:
			static const long MAX = 128L;

		public:
			guid::guid elementID;
			guid::guid componentID;
			char value[MAX];

		public:
			element() { clear(); }
			element(const element &source) { }
			
			void clear();
			
			bool bind(database::recordset *recordset);
			void set(data::element::element &source);
		};

		class request : public record::record <data::request::request>
		{
		public:
			guid::guid requestID;
			guid::guid guid;
			guid::guid user;
			guid::guid tag;
			TIMESTAMP_STRUCT created;

		public:
			request() { clear(); }
			request(const request &source) { }

			void clear();

			bool bind(database::recordset *recordset);
			void set(data::request::request &source);
		};

		class response : public record::record <data::response::response>
		{
		public:
			guid::guid responseID;
			guid::guid guid;
			guid::guid user;
			long status;
			TIMESTAMP_STRUCT created;
			bool available;

		public:
			response() { clear(); }
			response(const response &source) { }

			void clear();

			bool bind(database::recordset *recordset);
			void set(data::response::response &source);
		};
	};
};

#endif