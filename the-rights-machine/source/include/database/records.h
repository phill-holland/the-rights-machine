#include "database/interfaces/record.h"
#include "core/custom/datetime.h"
#include "core/custom/guid.h"
#include "message/message.h"
#include "models/item.h"
#include "models/line.h"
#include "models/component/line/component.h"
#include "models/component/query/component.h"
#include "models/request.h"
#include "models/response.h"

#if !defined(__RECORDS)
#define __RECORDS

namespace database
{
	namespace records
	{
		class message : public record::record<data::message::message>
		{
		public:
			core::custom::guid messageID;
			core::custom::guid user;
			core::custom::guid apikey;
			core::custom::guid guid;
			core::custom::guid tag;
			TIMESTAMP_STRUCT created;
			
		public:
			message() { clear(); }
			message(const message &source) { }

			void clear();

			bool bind(core::database::interface::recordset *recordset);
			void set(data::message::message &source);
		};

		class item : public record::record<models::item::item>
		{
		public:
			static const long MAX = 128L;

		public:
			core::custom::guid itemID;
			core::custom::guid messageID;
			char name[MAX];

		public:
			item() { clear(); }
			item(const item &source) { }
			
			void clear();

			bool bind(core::database::interface::recordset *recordset);
			void set(models::item::item &source);
		};

		class query : public record::record<models::query::query>
		{
		public:
			core::custom::guid queryID;
			core::custom::guid messageID;

		public:
			query() { clear(); }
			query(const query &source) { }

			void clear();

			bool bind(core::database::interface::recordset *recordset);
			void set(models::query::query &source);
		};

		class line : public record::record<models::line::line>
		{
		public:
			core::custom::guid lineID;
			core::custom::guid itemID;
			TIMESTAMP_STRUCT start;
			TIMESTAMP_STRUCT end;
			long exclusivityID;
			long typeID;

		public:
			line() { clear(); }
			line(const line &source) { }

			void clear();

			bool bind(core::database::interface::recordset *recordset);
			void set(models::line::line &source);
		};

		namespace component
		{
			namespace line
			{
				class component : public record::record<models::component::line::component>
				{
				public:
					static const long MAX = 128L;
					static const long TYPE = 2L;

				public:
					core::custom::guid componentID;
					core::custom::guid lineID;
					long type;
					char name[MAX];

				public:
					component() { clear(); }
					component(const component &source) { }

					void clear();

					bool bind(core::database::interface::recordset *recordset);
					void set(models::component::line::component &source);
				};
			};

			namespace query
			{
				class component : public record::record<models::component::query::component>
				{
				public:
					static const long MAX = 128L;
					static const long TYPE = 2L;

				public:
					core::custom::guid componentID;
					core::custom::guid queryID;
					long type;
					char name[MAX];

				public:
					component() { clear(); }
					component(const component &source) { }

					void clear();

					bool bind(core::database::interface::recordset *recordset);
					void set(models::component::query::component &source);
				};
			};
		};

		class element : public record::record<models::element::element>
		{
		public:
			static const long MAX = 128L;

		public:
			core::custom::guid elementID;
			core::custom::guid componentID;
			char value[MAX];

		public:
			element() { clear(); }
			element(const element &source) { }
			
			void clear();
			
			bool bind(core::database::interface::recordset *recordset);
			void set(models::element::element &source);
		};

		class request : public record::record <models::request::request>
		{
		public:
			core::custom::guid requestID;
			core::custom::guid guid;
			core::custom::guid user;
			core::custom::guid tag;
			TIMESTAMP_STRUCT created;

		public:
			request() { clear(); }
			request(const request &source) { }

			void clear();

			bool bind(core::database::interface::recordset *recordset);
			void set(models::request::request &source);
		};

		class response : public record::record <models::response::response>
		{
		public:
			core::custom::guid responseID;
			core::custom::guid guid;
			core::custom::guid user;
			long status;
			TIMESTAMP_STRUCT created;
			bool available;

		public:
			response() { clear(); }
			response(const response &source) { }

			void clear();

			bool bind(core::database::interface::recordset *recordset);
			void set(models::response::response &source);
		};
	};
};

#endif