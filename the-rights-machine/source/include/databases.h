#include "custom/string.h"
#include "datetime.h"

#if !defined(__DATABASES)
#define __DATABASES

namespace database
{
	class connection;

	class recordset
	{
		friend class connection;

	private:
		virtual bool create(void *source) = 0;

	public:
		virtual bool IsInitalised() = 0;

		virtual bool MoveNext() = 0;

		virtual long GetLong(long index) = 0;
		virtual string GetString(long index) = 0;
		virtual float GetFloat(long index) = 0;
		virtual double GetDouble(long index) = 0;
		virtual bool GetBool(long index) = 0;
		virtual TIMESTAMP_STRUCT GetTimeStamp(long index) = 0;
		//virtual GUID GetGUID(long index) = 0;

		virtual bool BindLong(long index, long &data) = 0;
		virtual bool BindString(long index, unsigned char *data) = 0;
		virtual bool BindFloat(long index, float &data) = 0;
		virtual bool BindDouble(long index, double &data) = 0;
		virtual bool BindBool(long index, bool &data) = 0;
		virtual bool BindTimeStamp(long index, TIMESTAMP_STRUCT &data) = 0;
		//virtual bool BindGUID(long index, GUID &data) = 0;

		virtual bool Execute() = 0;

		virtual void close() = 0;

	protected:
		virtual bool Execute(string &sql) = 0;
		virtual bool Execute(const char *sql) = 0;

		virtual bool Prepare(string &sql) = 0;
		virtual bool Prepare(const char *sql) = 0;
	};

	class connection
	{
	public:
		virtual bool initalised() = 0;

		virtual bool open(string &connection) = 0;
		virtual bool open(const char *connection) = 0;

		virtual bool close() = 0;

		virtual bool executeNoResults(string &sql) = 0;
		virtual bool executeWithResults(string &sql, recordset *result) = 0;
		virtual long executeScalar(string &sql) = 0;
		virtual bool Prepare(string &sql, recordset *result) = 0;

		virtual bool executeNoResults(const char *sql) = 0;
		virtual bool executeWithResults(const char *sql, recordset *result) = 0;
		virtual long executeScalar(const char *sql) = 0;
		virtual bool Prepare(const char *sql, recordset *result) = 0;
	};

	namespace factory
	{
		class recordset
		{
		public:
			virtual database::recordset *get() = 0;
		};

		class connection
		{
		public:
			virtual database::connection *get() = 0;
		};
	};

	class settings
	{
		factory::connection *connections;
		factory::recordset *recordsets;

		string location;

		bool init;

	public:
		settings(string location, factory::connection *connections, factory::recordset *recordsets) { makeNull(); reset(location, connections, recordsets); }
		
		bool initalised() { return init; }

		void reset(string location, factory::connection *connections, factory::recordset *recordsets)
		{
			init = false;

			this->location = location;
			this->connections = connections;
			this->recordsets = recordsets;

			init = true;
		}

		string getLocation() { return location; }
		
		factory::connection *getConnections() { return connections; }
		factory::recordset *getRecordSets() { return recordsets; }

	protected:
		void makeNull()
		{
			connections = NULL;
			recordsets = NULL;
		}
	};
};

#endif