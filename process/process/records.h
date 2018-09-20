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
			static const long MAX = 128L;

		public:
			GUID messageID;
			GUID user;
			GUID apikey;
			GUID guid;
			char created[MAX];
			char finished[MAX];

		public:
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
			bool bind(database::recordset *recordset);
			void set(data::item::item &source);
		};

		class line : public record::record<data::line::line>
		{
		public:
			static const long MAX = 128L;

		public:
			GUID lineID;
			GUID itemID;
			char start[MAX];
			char end[MAX];
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
					static const long MAX = 128L;

				public:
					GUID componentID;
					GUID lineID;
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
					static const long MAX = 128L;

				public:
					GUID componentID;
					GUID queryID;
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
			static const long MAX = 128L;

		public:
			GUID elementID;
			GUID componentID;
			char value[MAX];

		public:
			bool bind(database::recordset *recordset);
			void set(data::element::element &source);
		};
	};
};

#endif