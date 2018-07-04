#include "string.h"

#if !defined(__DATABASE)
#define __DATABASE

namespace database
{
	class connection;

	class recordset
	{
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

		virtual bool BindLong(long index, long &data) = 0;
		virtual bool BindString(long index, unsigned char *data) = 0;
		virtual bool BindFloat(long index, float &data) = 0;
		virtual bool BindDouble(long index, double &data) = 0;
		virtual bool BindBool(long index, bool &data) = 0;

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
		virtual bool executeWithResults(string &sql, recordset &result) = 0;
		virtual long executeScalar(string &sql) = 0;
		virtual bool Prepare(string &sql, recordset &result) = 0;

		virtual bool executeNoResults(const char *sql) = 0;
		virtual bool executeWithResults(const char *sql, recordset &result) = 0;
		virtual long executeScalar(const char *sql) = 0;
		virtual bool Prepare(const char *sql, recordset &result) = 0;
	};
};

#endif