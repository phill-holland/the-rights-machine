#include "record.h"
#include "datetime.h"
#include "message.h"
#include "item.h"
#include "line.h"
#include "component.h"
#include "request.h"
#include "response.h"

#if !defined(__RECORDS)
#define __RECORDS

namespace database
{
	namespace records
	{
		using namespace global;

		class message : public record::record<data::message::message>
		{
		public:
			GUID messageID;
			GUID user;
			GUID apikey;
			GUID guid;
			GUID tag;
			TIMESTAMP_STRUCT created;
			
		public:
			message() { clear(); }

			void clear();

			bool bind(database::recordset *recordset);
			void set(data::message::message &source);
		};

		class item : public record::record<data::item::item>
		{
		public:
			static const long MAX = 128L;

		public:
			GUID itemID;
			GUID messageID;
			char name[MAX];

		public:
			item() { clear(); }
			
			void clear();

			bool bind(database::recordset *recordset);
			void set(data::item::item &source);
		};

		class query : public record::record<data::query::query>
		{
		public:
			GUID queryID;
			GUID messageID;

		public:
			query() { clear(); }

			void clear();

			bool bind(database::recordset * recordset);
			void set(data::query::query &source);
		};

		class line : public record::record<data::line::line>
		{
		public:
			GUID lineID;
			GUID itemID;
			TIMESTAMP_STRUCT start;
			TIMESTAMP_STRUCT end;
			long exclusivityID;
			long typeID;

		public:
			line() { clear(); }

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
					GUID componentID;
					GUID lineID;
					long type;
					char name[MAX];

				public:
					component() { clear(); }

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
					GUID componentID;
					GUID queryID;
					long type;
					char name[MAX];

				public:
					component() { clear(); }
					
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
			GUID elementID;
			GUID componentID;
			char value[MAX];

		public:
			element() { clear(); }
			
			void clear();
			
			bool bind(database::recordset *recordset);
			void set(data::element::element &source);
		};

		class request : public record::record <data::request::request>
		{
		public:
			GUID requestID;
			GUID guid;
			GUID user;
			GUID tag;
			TIMESTAMP_STRUCT created;

		public:
			request() { clear(); }

			void clear();

			bool bind(database::recordset *recordset);
			void set(data::request::request &source);
		};

		class response : public record::record <data::response::response>
		{
		public:
			GUID responseID;
			GUID guid;
			GUID user;
			long status;
			TIMESTAMP_STRUCT created;
			bool available;

		public:
			response() { clear(); }

			void clear();

			bool bind(database::recordset *recordset);
			void set(data::response::response &source);
		};
	};
};

#endif