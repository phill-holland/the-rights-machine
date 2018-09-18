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
			long messageID;

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
			long itemID;
			long messageID;
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
			long lineID;
			long itemID;
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
					long componentID;
					long lineID;
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
				};
			};
		};

		class element : public record::record<data::element::element>
		{
		public:
			static const long MAX = 1024L;

		public:
			long elementID;
			long componentID;
			char value[MAX];

		public:
			bool bind(database::recordset *recordset);
			void set(data::element::element &source);
		};
	};
};

#endif