#include <windows.h>
#include <sqltypes.h>
#include <sql.h>
#include <sqlext.h>
#include "databases.h"
#include "string.h"
#include "datetime.h"
#include <vector>

#if !defined(__ODBC)
#define __ODBC

namespace database
{
	namespace odbc
	{
		using namespace global;

		class connection;

		class recordset : public ::database::recordset
		{
			friend class connection;

			SQLHANDLE lpStatement;

		private:
			bool create(void *source);

		public:
			recordset() { lpStatement = NULL; }
			~recordset() { cleanup(); }

			bool IsInitalised() { return lpStatement != NULL; }

			bool MoveNext();

			long GetLong(long index);
			string GetString(long index);
			float GetFloat(long index);
			double GetDouble(long index);
			bool GetBool(long index);
			TIMESTAMP_STRUCT GetTimeStamp(long index);
			GUID GetGUID(long index);

			bool BindLong(long index, long &data);
			bool BindString(long index, SQLCHAR *data);
			bool BindFloat(long index, float &data);
			bool BindDouble(long index, double &data);
			bool BindBool(long index, bool &data);
			bool BindTimeStamp(long index, TIMESTAMP_STRUCT &data);
			bool BindGUID(long index, GUID &data);

			bool Execute();

			void close() { cleanup(); }

			int logStatementError();

		protected:
			bool Execute(string &sql);
			bool Execute(const char *sql);

			bool Prepare(string &sql);
			bool Prepare(const char *sql);

		protected:
			void cleanup();

		public:
			recordset operator=(const recordset &src)
			{
				cleanup(); lpStatement = src.lpStatement;
				return *this;
			}
		};

		class connection : public ::database::connection
		{
			SQLHANDLE lpEnvironment;
			SQLHANDLE lpConnection;

			bool init, isopen;

		public:
			connection() { makeNull(); reset(); };
			~connection() { cleanup(); }

			bool initalised() { return init; }

			void reset();

			bool open(string &connection);
			bool open(const char *connection);

			bool close();

			bool executeNoResults(string &sql);
			bool executeWithResults(string &sql, database::recordset *result);
			long executeScalar(string &sql);
			bool Prepare(string &sql, database::recordset *result);

			bool executeNoResults(const char *sql);
			bool executeWithResults(const char *sql, database::recordset *result);
			long executeScalar(const char *sql);
			bool Prepare(const char *sql, database::recordset *result);

			int logConnectionError();

		protected:
			void makeNull();
			void cleanup();
		};

		namespace factory
		{
			class connection : public database::factory::connection
			{
				std::vector<odbc::connection*> connections;

				bool init;

			public:
				connection() { makeNull(); reset(); }
				~connection() { cleanup(); }

				bool initalised() { return init; }
				void reset();

				database::connection *get();

			protected:
				void makeNull();
				void cleanup();
			};

			class recordset : public database::factory::recordset
			{
				std::vector<odbc::recordset*> recordsets;

				bool init;

			public:
				recordset() { makeNull(); reset(); }
				~recordset() { cleanup(); }

				bool initalised() { return init; }
				void reset();

				database::recordset *get();
				
			protected:
				void makeNull();
				void cleanup();
			};
		};
	};
};

#endif