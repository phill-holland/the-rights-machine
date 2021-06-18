#include "record.h"
#include "records.h"
#include "databases.h"
#include "odbc.h"
#include "message.h"
#include "request.h"
#include "response.h"
#include "comparison.h"
#include "file.h"
#include "guid.h"
#include <vector>
#include <unordered_map>

#if !defined(__STORAGE)
#define __STORAGE

namespace database
{
	namespace storage
	{
		using namespace comparison;

		class request : public file::file<data::request::request>
		{
		public:
			static const long TOP = 200L;

		protected:
			database::records::request bound;

			database::connection *connection;
			database::recordset *recordset;

			std::vector<database::records::request> data;

		public:
			std::vector<string> identities;

		public:
			long max;

		public:
			request()
			{
				max = TOP;

				connection = NULL;
				recordset = NULL;
			}

			bool open(database::settings &settings);
			bool close();

			bool read(data::request::request &destination);
			bool write(data::request::request &source);

			void clear() { data.clear(); identities.clear(); }

		protected:
			bool load();

		protected:
			bool tag(guid::guid &tagged);
			bool erase(guid::guid &tagged);
		};

		class response : public file::file<data::response::response>
		{
		public:
			static const long TOP = 200L;

		protected:
			database::records::response bound;

			database::connection *connection;
			database::recordset *recordset;

			std::vector<database::records::response> data;

		public:
			std::vector<string> identities;

		public:
			long max;

		public:
			response()
			{
				max = TOP;

				connection = NULL;
				recordset = NULL;
			}

			bool open(database::settings &settings);
			bool close();

			bool read(data::response::response &destination);
			bool write(data::response::response &source);

			void clear() { data.clear(); identities.clear(); }

		protected:
			bool load();
		};

		namespace common
		{
			namespace line
			{
				class element : public file::file<data::element::element>
				{
					database::records::element bound;

					database::connection *connection;
					database::recordset *recordset;

					std::unordered_map<string, std::vector<database::records::element>, hasher, equality> data;

				public:
					data::message::message *parent;
					std::vector<string> identities;
					guid::guid componentID;

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

					element t_element;

					std::unordered_map<string, std::vector<database::records::component::line::component>, hasher, equality> data;

				public:
					data::message::message *parent;
					std::vector<string> identities;
					guid::guid lineID;

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

					void clear() { data.clear(); t_element.clear(); identities.clear(); }

				protected:
					bool load();
				};
			};

			namespace query
			{
				class element : public file::file<data::element::element>
				{
					database::records::element bound;

					database::connection *connection;
					database::recordset *recordset;

					std::unordered_map<string, std::vector<database::records::element>, hasher, equality> data;

				public:
					data::query::query *parent;
					std::vector<string> identities;
					guid::guid componentID;

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

				class component : public file::file<data::component::query::component>
				{
					database::records::component::query::component bound;

					database::connection *connection;
					database::recordset *recordset;

					element t_element;

					std::unordered_map<string, std::vector<database::records::component::query::component>, hasher, equality> data;

				public:
					data::query::query *parent;
					std::vector<string> identities;
					guid::guid queryID;

				public:
					component()
					{
						parent = NULL;
						connection = NULL;
						recordset = NULL;
					}

					bool open(database::settings &settings);
					bool close();

					bool read(data::component::query::component &destination);
					bool write(data::component::query::component &source);

					void clear() { data.clear(); t_element.clear(); identities.clear(); }

				protected:
					bool load();
				};
			};
		};

		class line : public file::file<data::line::line>
		{
			database::records::line bound;

			database::connection *connection;
			database::recordset *recordset;

			common::line::component component;

			std::unordered_map<string, std::vector<database::records::line>, hasher, equality> data;

		public:
			data::message::message *parent;
			std::vector<string> identities;
			guid::guid itemID;

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

		class query : public file::file<data::query::query>
		{
			database::records::query bound;

			database::connection *connection;
			database::recordset *recordset;

			common::query::component component;

			std::unordered_map<string, std::vector<database::records::query>, hasher, equality> data;

		public:
			data::message::message *parent;
			std::vector<string> identities;
			guid::guid messageID;

		public:
			query()
			{
				parent = NULL;
				connection = NULL;
				recordset = NULL;
			}

			bool open(database::settings &settings);
			bool close();

			bool read(data::query::query &destination);
			bool write(data::query::query &source);

			void clear() { data.clear(); component.clear(); identities.clear(); }

		protected:
			bool load();
		};
		

		class item : public file::file<data::item::item>
		{
			database::records::item bound;

			database::connection *connection;
			database::recordset *recordset;

			line t_line;

			std::unordered_map<string, std::vector<database::records::item>, hasher, equality> data;

		public:
			data::message::message *parent;
			std::vector<string> identities;
			guid::guid messageID;
			
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

			void clear() { data.clear(); t_line.clear(); identities.clear(); }

		protected:
			bool load();
		};

		class message : public file::file<data::message::message>
		{
		public:
			static const long TOP = 200L;

		private:
			database::records::message bound;

			database::connection *connection;
			database::recordset *recordset;

			item t_item;
			query t_query;

			std::vector<database::records::message> data;
			std::vector<string> identities;
			
		public:
			long max;

		public:
			message() 
			{ 
				max = TOP;
			
				connection = NULL;
				recordset = NULL;
			}

			bool open(database::settings &settings);
			bool close();

			bool read(data::message::message &destination);
			bool write(data::message::message &source);

			void clear() { data.clear(); t_item.clear(); identities.clear(); }

		public:
			bool load();

		protected:
			bool tag(guid::guid &tagged);
			bool erase(guid::guid &tagged);
		};
	};
};

#endif