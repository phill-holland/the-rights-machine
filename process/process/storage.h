#include "record.h"
#include "records.h"
#include "databases.h"
#include "odbc.h"
#include "message.h"
#include "comparison.h"
#include "file.h"
#include <vector>
#include <unordered_map>

#if !defined(__STORAGE)
#define __STORAGE

// change primary keys into uniqueidentifiers - GUIDS/strings
// implement query load
// implement save, generate GUIDS so database doesn't need a read during save
// need to mark messageID's (from list of identities as read, or delete from DB

namespace database
{
	namespace storage
	{
		using namespace comparison;

		class element : public file::file<data::element::element>
		{
			database::records::element bound;

			database::connection *connection;
			database::recordset *recordset;

			std::unordered_map<string, std::vector<database::records::element>, hasher, equality> data;

		public:
			data::message::message *parent;
			std::vector<string> identities;
			GUID componentID; 

		public:
			element()
			{
				parent = NULL;
				connection = NULL;
				recordset = NULL;
			}

			bool open(database::settings &settings);
			bool close();

			bool read(data::element::element &destination);
			bool write(data::element::element &source);

			void clear() { data.clear(); identities.clear(); }

		protected:
			bool load();
		};

		class component : public file::file<data::component::line::component>
		{
			database::records::component::line::component bound;

			database::connection *connection;
			database::recordset *recordset;

			element element;

			std::unordered_map<string, std::vector<database::records::component::line::component>, hasher, equality> data;

		public:
			data::message::message *parent;
			std::vector<string> identities;
			GUID lineID;

		public:
			component()
			{
				parent = NULL;
				connection = NULL;
				recordset = NULL;
			}

			bool open(database::settings &settings);
			bool close();

			bool read(data::component::line::component &destination);
			bool write(data::component::line::component &source);

			void clear() { data.clear(); element.clear(); identities.clear(); }

		protected:
			bool load();
		};

		class line : public file::file<data::line::line>
		{
			database::records::line bound;

			database::connection *connection;
			database::recordset *recordset;

			component component;

			std::unordered_map<string, std::vector<database::records::line>, hasher, equality> data;
			
		public:
			data::message::message *parent;
			std::vector<string> identities;
			GUID itemID;

		public:
			line() 
			{
				parent = NULL;
				connection = NULL;
				recordset = NULL;				
			}

			bool open(database::settings &settings);
			bool close();

			bool read(data::line::line &destination);
			bool write(data::line::line &source);

			void clear() { data.clear(); component.clear(); identities.clear(); }

		protected:
			bool load();
		};

		class item : public file::file<data::item::item>
		{
			database::records::item bound;

			database::connection *connection;
			database::recordset *recordset;

			line line;

			std::unordered_map<string, std::vector<database::records::item>, hasher, equality> data;

		public:
			data::message::message *parent;
			std::vector<string> identities;
			GUID messageID;
			
		public:
			item()
			{ 
				parent = NULL;
				connection = NULL;
				recordset = NULL;
			}

			bool open(database::settings &settings);
			bool close();

			bool read(data::item::item &destination);
			bool write(data::item::item &source);

			void clear() { data.clear(); line.clear(); identities.clear(); }

		protected:
			bool load();
		};

		class message : public file::file<data::message::message>
		{
			database::records::message bound;

			database::connection *connection;
			database::recordset *recordset;

			item item;

			std::vector<database::records::message> data;
			std::vector<string> identities;

		public:
			long max;

		public:
			message() { max = 0L; }

			bool open(database::settings &settings);
			bool close();

			bool read(data::message::message &destination);
			bool write(data::message::message &source);

			void clear() { data.clear(); item.clear(); identities.clear(); }

		public:
			bool load();			
		};
	};
};

#endif