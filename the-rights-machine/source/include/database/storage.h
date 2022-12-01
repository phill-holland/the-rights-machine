#include "database/interfaces/record.h"
#include "database/records.h"
#include "core/database/connection.h"
#include "core/database/recordset.h"
#include "message/message.h"
#include "models/request.h"
#include "models/response.h"
#include "core/string/comparison.h"
#include "database/interfaces/file.h"
#include "core/custom/guid.h"
#include <vector>
#include <unordered_map>

#if !defined(__STORAGE)
#define __STORAGE

namespace database
{
	namespace storage
	{
		using namespace comparison;

		class request : public file::file<models::request::request>
		{
		public:
			static const long TOP = 200L;

		protected:
			database::records::request bound;

			core::database::interface::connection *connection;
			core::database::interface::recordset *recordset;

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

			bool open(core::database::settings &settings);
			bool close();

			bool read(models::request::request &destination);
			bool write(models::request::request &source);

			void clear() { data.clear(); identities.clear(); }

		protected:
			bool load();

		protected:
			bool tag(core::custom::guid &tagged);
			bool erase(core::custom::guid &tagged);
		};

		class response : public file::file<models::response::response>
		{
		public:
			static const long TOP = 200L;

		protected:
			database::records::response bound;

			core::database::interface::connection *connection;
			core::database::interface::recordset *recordset;

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

			bool open(core::database::settings &settings);
			bool close();

			bool read(models::response::response &destination);
			bool write(models::response::response &source);

			void clear() { data.clear(); identities.clear(); }

		protected:
			bool load();
		};

		namespace common
		{
			namespace line
			{
				class element : public file::file<models::element::element>
				{
					database::records::element bound;

					core::database::interface::connection *connection;
					core::database::interface::recordset *recordset;

					std::unordered_map<string, std::vector<database::records::element>, hasher, equality> data;

				public:
					data::message::message *parent;
					std::vector<string> identities;
					core::custom::guid componentID;

				public:
					element()
					{
						parent = NULL;
						connection = NULL;
						recordset = NULL;
					}

					bool open(core::database::settings &settings);
					bool close();

					bool read(models::element::element &destination);
					bool write(models::element::element &source);

					void clear() { data.clear(); identities.clear(); }

				protected:
					bool load();
				};

				class component : public file::file<models::component::line::component>
				{
					database::records::component::line::component bound;

					core::database::interface::connection *connection;
					core::database::interface::recordset *recordset;

					element t_element;

					std::unordered_map<string, std::vector<database::records::component::line::component>, hasher, equality> data;

				public:
					data::message::message *parent;
					std::vector<string> identities;
					core::custom::guid lineID;

				public:
					component()
					{
						parent = NULL;
						connection = NULL;
						recordset = NULL;
					}

					bool open(core::database::settings &settings);
					bool close();

					bool read(models::component::line::component &destination);
					bool write(models::component::line::component &source);

					void clear() { data.clear(); t_element.clear(); identities.clear(); }

				protected:
					bool load();
				};
			};

			namespace query
			{
				class element : public file::file<models::element::element>
				{
					database::records::element bound;

					core::database::interface::connection *connection;
					core::database::interface::recordset *recordset;

					std::unordered_map<string, std::vector<database::records::element>, hasher, equality> data;

				public:
					models::query::query *parent;
					std::vector<string> identities;
					core::custom::guid componentID;

				public:
					element()
					{
						parent = NULL;
						connection = NULL;
						recordset = NULL;
					}

					bool open(core::database::settings &settings);
					bool close();

					bool read(models::element::element &destination);
					bool write(models::element::element &source);

					void clear() { data.clear(); identities.clear(); }

				protected:
					bool load();
				};

				class component : public file::file<models::component::query::component>
				{
					database::records::component::query::component bound;

					core::database::interface::connection *connection;
					core::database::interface::recordset *recordset;

					element t_element;

					std::unordered_map<string, std::vector<database::records::component::query::component>, hasher, equality> data;

				public:
					models::query::query *parent;
					std::vector<string> identities;
					core::custom::guid queryID;

				public:
					component()
					{
						parent = NULL;
						connection = NULL;
						recordset = NULL;
					}

					bool open(core::database::settings &settings);
					bool close();

					bool read(models::component::query::component &destination);
					bool write(models::component::query::component &source);

					void clear() { data.clear(); t_element.clear(); identities.clear(); }

				protected:
					bool load();
				};
			};
		};

		class line : public file::file<models::line::line>
		{
			database::records::line bound;

			core::database::interface::connection *connection;
			core::database::interface::recordset *recordset;

			common::line::component component;

			std::unordered_map<string, std::vector<database::records::line>, hasher, equality> data;

		public:
			data::message::message *parent;
			std::vector<string> identities;
			core::custom::guid itemID;

		public:
			line()
			{
				parent = NULL;
				connection = NULL;
				recordset = NULL;
			}

			bool open(core::database::settings &settings);
			bool close();

			bool read(models::line::line &destination);
			bool write(models::line::line &source);

			void clear() { data.clear(); component.clear(); identities.clear(); }

		protected:
			bool load();
		};

		class query : public file::file<models::query::query>
		{
			database::records::query bound;

			core::database::interface::connection *connection;
			core::database::interface::recordset *recordset;

			common::query::component component;

			std::unordered_map<string, std::vector<database::records::query>, hasher, equality> data;

		public:
			data::message::message *parent;
			std::vector<string> identities;
			core::custom::guid messageID;

		public:
			query()
			{
				parent = NULL;
				connection = NULL;
				recordset = NULL;
			}

			bool open(core::database::settings &settings);
			bool close();

			bool read(models::query::query &destination);
			bool write(models::query::query &source);

			void clear() { data.clear(); component.clear(); identities.clear(); }

		protected:
			bool load();
		};
		

		class item : public file::file<models::item::item>
		{
			database::records::item bound;

			core::database::interface::connection *connection;
			core::database::interface::recordset *recordset;

			line t_line;

			std::unordered_map<string, std::vector<database::records::item>, hasher, equality> data;

		public:
			data::message::message *parent;
			std::vector<string> identities;
			core::custom::guid messageID;
			
		public:
			item()
			{ 
				parent = NULL;
				connection = NULL;
				recordset = NULL;
			}

			bool open(core::database::settings &settings);
			bool close();

			bool read(models::item::item &destination);
			bool write(models::item::item &source);

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

			core::database::interface::connection *connection;
			core::database::interface::recordset *recordset;

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

			bool open(core::database::settings &settings);
			bool close();

			bool read(data::message::message &destination);
			bool write(data::message::message &source);

			void clear() { data.clear(); t_item.clear(); identities.clear(); }

		public:
			bool load();

		protected:
			bool tag(core::custom::guid &tagged);
			bool erase(core::custom::guid &tagged);
		};
	};
};

#endif