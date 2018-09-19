#include "record.h"
#include "datetime.h"
#include "message.h"
#include "item.h"
#include "line.h"
#include "component.h"

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
			static const long MAX = 1024L;

		public:
			char messageID[MAX];

			char user[MAX];
			char APIKey[MAX];
			char GUID[MAX];
			
			TIMESTAMP_STRUCT created;
			TIMESTAMP_STRUCT finished;

		public:
			bool bind(database::recordset *recordset);
			void set(data::message::message &source);
		};

		class item : public record::record<data::item::item>
		{
		public:
			static const long MAX = 1024L;

		public:
			char itemID[MAX];
			char messageID[MAX];
			char name[MAX];

		public:
			bool bind(database::recordset *recordset);
			void set(data::item::item &source);
		};

		class line : public record::record<data::line::line>
		{
		public:
			static const long MAX = 1024L;

		public:
			char lineID[MAX];
			char itemID[MAX];
			TIMESTAMP_STRUCT start;
			TIMESTAMP_STRUCT end;
			long exclusivityID;
			long typeID;

		public:
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
					static const long MAX = 1024L;

				public:
					char componentID[MAX];
					char lineID[MAX];
					char name[MAX];

				public:
					bool bind(database::recordset *recordset);
					void set(data::component::line::component &source);
				};
			};

			namespace query
			{
				class component : public record::record<data::component::query::component>
				{
				public:
					static const long MAX = 1024L;

				public:
					char componentID[MAX];
					char queryID[MAX];
					char name[MAX];

				public:
					bool bind(database::recordset *recordset);
					void set(data::component::query::component &source);
				};
			};
		};

		class element : public record::record<data::element::element>
		{
		public:
			static const long MAX = 1024L;

		public:
			char elementID[MAX];
			char componentID[MAX];
			char value[MAX];

		public:
			bool bind(database::recordset *recordset);
			void set(data::element::element &source);
		};
	};
};

#endif