#include "odbc.h"
#include "custom/string.h"
#include "log.h"
#include <cstring>
/*
bool odbc::recordset::create(SQLHANDLE &lpConnection)
{
	cleanup();

	SQLRETURN result = SQLAllocHandle(SQL_HANDLE_STMT, lpConnection, &lpStatement);
	if (result != SQL_ERROR) return true;

	lpStatement = NULL;

	return false;
}
*/
/*
bool odbc::recordset::create(void *source)
{
	cleanup();

	SQLHANDLE lpConnection = (SQLHANDLE *)source;
	SQLRETURN result = SQLAllocHandle(SQL_HANDLE_STMT, lpConnection, &lpStatement);
	if (result != SQL_ERROR) return true;

	lpStatement = NULL;

	return false;
}

bool odbc::recordset::MoveNext()
{
	if (lpStatement == NULL) return false;
	SQLRETURN result = SQLFetch(lpStatement);
	bool ok = ((result == SQL_SUCCESS) || (result == SQL_SUCCESS_WITH_INFO));
	if (!ok)
	{
		if (SQLFreeHandle(SQL_HANDLE_STMT, lpStatement) == SQL_SUCCESS)
			lpStatement = NULL;
	}
	return ok;
}

long odbc::recordset::GetLong(long index)
{
	int result = -1;
	SQLRETURN re = SQLGetData(lpStatement, (SQLUSMALLINT)index, SQL_C_LONG, &result, 0, NULL);
	return (long)result;
}

string odbc::recordset::GetString(long index)
{
	char buffer[100];
	SQLGetData(lpStatement, (SQLUSMALLINT)index, SQL_C_CHAR, buffer, 100, NULL);
	return string(buffer);
}

float odbc::recordset::GetFloat(long index)
{
	float result = 0.0f;
	SQLGetData(lpStatement, (SQLUSMALLINT)index, SQL_C_FLOAT, &result, 0, NULL);
	return result;
}

double odbc::recordset::GetDouble(long index)
{
	double result = 0.0;
	SQLGetData(lpStatement, (SQLUSMALLINT)index, SQL_C_DOUBLE, &result, 0, NULL);
	return result;
}

bool odbc::recordset::GetBool(long index)
{
	bool result = false;
	SQLGetData(lpStatement, (SQLUSMALLINT)index, SQL_C_BIT, &result, 0, NULL);
	return result;
}

bool odbc::recordset::BindLong(long index, long &data)
{
	if (SQLBindParameter(lpStatement, (SQLUSMALLINT)index, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &data, 0, NULL) == SQL_SUCCESS)
		return true;

	return false;
}

bool odbc::recordset::BindString(long index, SQLCHAR *data)
{
	if (SQLBindParameter(lpStatement, (SQLUSMALLINT)index, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_LONGVARCHAR, strlen((char*)data), 0, data, strlen((char*)data), NULL) == SQL_SUCCESS)
		return true;

	return false;
}

bool odbc::recordset::BindFloat(long index, float &data)
{
	if (SQLBindParameter(lpStatement, (SQLUSMALLINT)index, SQL_PARAM_INPUT, SQL_C_FLOAT, SQL_FLOAT, 0, 0, &data, 0, NULL) == SQL_SUCCESS)
		return true;
	return false;
}

bool odbc::recordset::BindDouble(long index, double &data)
{
	if (SQLBindParameter(lpStatement, (SQLUSMALLINT)index, SQL_PARAM_INPUT, SQL_C_DOUBLE, SQL_DOUBLE, 0, 0, &data, 0, NULL) == SQL_SUCCESS)
		return true;
	return false;
}

bool odbc::recordset::BindBool(long index, bool &data)
{
	if (SQLBindParameter(lpStatement, (SQLUSMALLINT)index, SQL_PARAM_INPUT, SQL_C_BIT, SQL_BIT, 0, 0, &data, 0, NULL) == SQL_SUCCESS)
		return true;
	return false;
}

bool odbc::recordset::BindLongColumn(long index, int *source, long length)
{
  	SQLLEN len = (SQLLEN)length;

	if (SQLSetStmtAttr(lpStatement, SQL_ATTR_PARAM_BIND_TYPE, SQL_PARAM_BIND_BY_COLUMN, 0) != SQL_SUCCESS) return false;

  	if (SQLSetStmtAttr(lpStatement, SQL_ATTR_PARAMSET_SIZE, (void *)len, 0) != SQL_SUCCESS)  return false;

	if (SQLBindParameter(lpStatement, (SQLUSMALLINT)index, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, source, 0, 0) != SQL_SUCCESS) return false;

	return true;
}

bool odbc::recordset::BindFloatColumn(long index, float *source, long length)
{
  	SQLLEN len = (SQLLEN)length;

	if (SQLSetStmtAttr(lpStatement, SQL_ATTR_PARAM_BIND_TYPE, SQL_PARAM_BIND_BY_COLUMN, 0) != SQL_SUCCESS) return false;

  	if (SQLSetStmtAttr(lpStatement, SQL_ATTR_PARAMSET_SIZE, (void *)len, 0) != SQL_SUCCESS)  return false;

	if (SQLBindParameter(lpStatement, (SQLUSMALLINT)index, SQL_PARAM_INPUT, SQL_C_FLOAT, SQL_FLOAT, 0, 0, source, 0, 0) != SQL_SUCCESS) return false;

	return true;
}

bool odbc::recordset::Execute()
{
	if (lpStatement == NULL) return false;
	SQLRETURN result = SQLExecute(lpStatement);

	return ((result == SQL_SUCCESS) || (result == SQL_SUCCESS_WITH_INFO));
}

string odbc::recordset::getStatementError()
{
	SQLCHAR SqlState[6], Msg[SQL_MAX_MESSAGE_LENGTH];
	SQLINTEGER NativeError;
	SQLSMALLINT i = 1, MsgLen;

	string result;

	while (SQLGetDiagRec(SQL_HANDLE_STMT, lpStatement, i, SqlState, &NativeError, Msg, sizeof(Msg), &MsgLen) != SQL_NO_DATA)
	{
		result.concat(string((char*)SqlState));
		result.concat(string((char*)Msg));
		result.concat(string("\n"));

		i++;
	}

	return result;
}

bool odbc::recordset::Execute(string sql)
{
	if (lpStatement == NULL) return false;
	SQLRETURN result = SQLExecDirect(lpStatement, (SQLCHAR*)sql.c_str(), SQL_NTS);
	return ((result == SQL_SUCCESS) || (result == SQL_SUCCESS_WITH_INFO));
}

bool odbc::recordset::Prepare(string sql)
{
	if (lpStatement == NULL) return false;

	const char *temp = sql.c_str();
	SQLRETURN result = SQLPrepare(lpStatement, (SQLCHAR*)temp, (SQLINTEGER)strlen(temp));

	return result != SQL_ERROR;
}

void odbc::recordset::cleanup()
{
	if (lpStatement != NULL)
	{
		try
		{
			SQLFreeHandle(SQL_HANDLE_STMT, lpStatement);
		}
		catch(...)
		{
			Log << string("odbc::recordset::cleanup SEH Error\r\n");
		}

		lpStatement = NULL;
	}
}

void odbc::connection::reset()
{
	isopen = false; init = false; cleanup();

	if (SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &lpEnvironment) == SQL_ERROR) return;
	if (SQLSetEnvAttr(lpEnvironment, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, 0) != SQL_SUCCESS) return;
	if (SQLAllocHandle(SQL_HANDLE_DBC, lpEnvironment, &lpConnection) != SQL_SUCCESS) return;

	SQLSetConnectAttr(lpConnection, SQL_LOGIN_TIMEOUT, (SQLPOINTER)120, 0);

	init = true;
}

bool odbc::connection::open(string connection)
{
	return open(connection.c_str());
}

bool odbc::connection::open(const char *connection)
{
	SQLCHAR out[255];
	SQLSMALLINT len;

	SQLRETURN result = SQLDriverConnect(lpConnection, NULL, (SQLCHAR*)connection, SQL_NTS, out, 255, &len, SQL_DRIVER_NOPROMPT);

	if (result == SQL_SUCCESS || result == SQL_SUCCESS_WITH_INFO)
	{
		isopen = true;
		return true;
	}

	return false;
}

bool odbc::connection::close()
{
	if (!isopen) return false;
	bool result = true;

	if (lpConnection != NULL)
	{
		SQLRETURN sqlret = SQLDisconnect(lpConnection);
		if ((result != SQL_SUCCESS) && (result != SQL_SUCCESS_WITH_INFO)) result = false;
		if (SQLFreeHandle(SQL_HANDLE_DBC, lpConnection) != SQL_SUCCESS) result = false;
	}

	if (lpEnvironment != NULL) if (SQLFreeHandle(SQL_HANDLE_ENV, lpEnvironment) != SQL_SUCCESS) result = false;

	isopen = false;
	return result;
}

bool odbc::connection::executeNoResults(string sql)
{
	if (!isopen) return false;
	bool result = false;

	try
	{
		SQLHANDLE lpStatement = NULL;
		if (SQLAllocHandle(SQL_HANDLE_STMT, lpConnection, &lpStatement) == SQL_ERROR) return false;
		if (SQLExecDirect(lpStatement, (SQLCHAR*)sql.c_str(), SQL_NTS) != SQL_ERROR) result = true;
		if (SQLFreeHandle(SQL_HANDLE_STMT, lpStatement) != SQL_SUCCESS) result = false;
	}
	catch (...) { return false; }

	return result;
}

bool odbc::connection::executeNoResults(string sql, string &error)
{
	if (!isopen) return false;
	bool result = false;

	error.clear();

	try
	{
		recordset temp;
		if(temp.create(lpConnection))
		{
			result = temp.Execute(sql);
			if(!result) error = temp.getStatementError();
		}
	}
	catch (...) { return false; }

	return result;
}

bool odbc::connection::executeWithResults(string sql, database::recordset *result)
{
	if(result->create(lpConnection))
	{
		return result->Execute(sql);
	}

	return false;
}

long odbc::connection::executeScalar(string sql)
{
	long result = -1L;
	if (!isopen) return result;

	try
	{
		SQLHANDLE lpStatement = NULL;
		if (SQLAllocHandle(SQL_HANDLE_STMT, lpConnection, &lpStatement) == SQL_ERROR) return result;
		if (SQLExecDirect(lpStatement, (SQLCHAR*)sql.c_str(), SQL_NTS) != SQL_ERROR)
		{
			if (SQLFetch(lpStatement) == SQL_SUCCESS)
			{
				SQLINTEGER temp = 0;
				SQLGetData(lpStatement, 1, SQL_C_LONG, &temp, 0, NULL);
				result = (long)temp;
			}
		}
		if (SQLFreeHandle(SQL_HANDLE_STMT, lpStatement) != SQL_SUCCESS) result = -1L;
	}
	catch (...) { return -1L; }

	return result;
}

bool odbc::connection::Prepare(string sql, database::recordset *result)
{
	if(result->create(lpConnection))
	{
		return result->Prepare(sql);
	}

	return false;
}

string odbc::connection::getConnectionError()
{
	SQLCHAR SqlState[6], Msg[SQL_MAX_MESSAGE_LENGTH];
	SQLINTEGER NativeError;
	SQLSMALLINT i = 1, MsgLen;

	string result;

	while (SQLGetDiagRec(SQL_HANDLE_DBC, lpConnection, i, SqlState, &NativeError, Msg, sizeof(Msg), &MsgLen) != SQL_NO_DATA)
	{
		result.concat(string((char*)SqlState));
		result.concat(string((char*)Msg));
		result.concat(string("\n"));

		i++;
	}

	return result;
}


void odbc::connection::makeNull()
{
	lpConnection = NULL;
	lpEnvironment = NULL;
}

void odbc::connection::cleanup()
{
	if (isopen) close();
}
*/

#include "odbc.h"
#include "custom/string.h"
#include "log.h"

bool database::odbc::recordset::create(void *source)
{
	cleanup();

	SQLHANDLE lpConnection = (SQLHANDLE *)source;
	SQLRETURN result = SQLAllocHandle(SQL_HANDLE_STMT, lpConnection, &lpStatement);
	if (result != SQL_ERROR) return true;

	lpStatement = NULL;

	return false;
}

bool database::odbc::recordset::MoveNext()
{
	if (lpStatement == NULL) return false;
	SQLRETURN result = SQLFetch(lpStatement);
	bool ok = ((result == SQL_SUCCESS) || (result == SQL_SUCCESS_WITH_INFO));
	if (!ok)
	{
		if (SQLFreeHandle(SQL_HANDLE_STMT, lpStatement) == SQL_SUCCESS)
			lpStatement = NULL;
	}
	return ok;
}

long database::odbc::recordset::GetLong(long index)
{
	long result = -1L;
	SQLGetData(lpStatement, (SQLUSMALLINT)index, SQL_C_LONG, &result, 0, NULL);
	return result;
}

string database::odbc::recordset::GetString(long index)
{
	char buffer[100];
	SQLGetData(lpStatement, (SQLUSMALLINT)index, SQL_C_CHAR, buffer, 100, NULL);
	return string(buffer);
}

float database::odbc::recordset::GetFloat(long index)
{
	float result = 0.0f;
	SQLGetData(lpStatement, (SQLUSMALLINT)index, SQL_C_FLOAT, &result, 0, NULL);
	return result;
}

double database::odbc::recordset::GetDouble(long index)
{
	double result = 0.0;
	SQLGetData(lpStatement, (SQLUSMALLINT)index, SQL_C_DOUBLE, &result, 0, NULL);
	return result;
}

bool database::odbc::recordset::GetBool(long index)
{
	bool result = false;
	SQLGetData(lpStatement, (SQLUSMALLINT)index, SQL_C_BIT, &result, 0, NULL);
	return result;
}

TIMESTAMP_STRUCT database::odbc::recordset::GetTimeStamp(long index)
{
	TIMESTAMP_STRUCT ts = { 0 };

	SQLGetData(lpStatement, (SQLUSMALLINT)index, SQL_C_TIMESTAMP, &ts, 0, NULL);

	return ts;
}

guid::guid database::odbc::recordset::GetGUID(long index)
{
	guid::guid result;
#warning this is totaly going to break
	SQLGetData(lpStatement, (SQLUSMALLINT)index, SQL_C_GUID, &result, 0, NULL);

	return result;
}

bool database::odbc::recordset::BindLong(long index, long &data)
{
	if (SQLBindParameter(lpStatement, (SQLUSMALLINT)index, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &data, 0, NULL) == SQL_SUCCESS)
		return true;
	return false;
}

bool database::odbc::recordset::BindString(long index, SQLCHAR *data)
{
	if (SQLBindParameter(lpStatement, (SQLUSMALLINT)index, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_LONGVARCHAR, strlen((char*)data), 0, data, strlen((char*)data), NULL) == SQL_SUCCESS)
		return true;
	return false;
}

bool database::odbc::recordset::BindFloat(long index, float &data)
{
	if (SQLBindParameter(lpStatement, (SQLUSMALLINT)index, SQL_PARAM_INPUT, SQL_C_FLOAT, SQL_FLOAT, 0, 0, &data, 0, NULL) == SQL_SUCCESS)
		return true;
	return false;
}

bool database::odbc::recordset::BindDouble(long index, double &data)
{
	if (SQLBindParameter(lpStatement, (SQLUSMALLINT)index, SQL_PARAM_INPUT, SQL_C_DOUBLE, SQL_DOUBLE, 0, 0, &data, 0, NULL) == SQL_SUCCESS)
		return true;
	return false;
}

bool database::odbc::recordset::BindBool(long index, bool &data)
{
	if (SQLBindParameter(lpStatement, (SQLUSMALLINT)index, SQL_PARAM_INPUT, SQL_C_BIT, SQL_BIT, 0, 0, &data, 0, NULL) == SQL_SUCCESS)
		return true;
	return false;
}

bool database::odbc::recordset::BindTimeStamp(long index, TIMESTAMP_STRUCT &data)
{
	if (SQLBindParameter(lpStatement, (SQLUSMALLINT)index, SQL_PARAM_INPUT, SQL_C_TIMESTAMP, SQL_TIMESTAMP, 0, 0, &data, 0, NULL) == SQL_SUCCESS)
		return true;
	return false;
}

bool database::odbc::recordset::BindGUID(long index, guid::guid &data)
{
#warning breaky breaky
	if (SQLBindParameter(lpStatement, (SQLUSMALLINT)index, SQL_PARAM_INPUT, SQL_C_GUID, SQL_GUID, 0, 0, &data, 0, NULL) == SQL_SUCCESS)
		return true;
	return false;
}

bool database::odbc::recordset::Execute()
{
	if (lpStatement == NULL) return false;
	SQLRETURN result = SQLExecute(lpStatement);
	logStatementError();
	return ((result == SQL_SUCCESS) || (result == SQL_SUCCESS_WITH_INFO));
}

int database::odbc::recordset::logStatementError()
{
	SQLCHAR SqlState[6], Msg[SQL_MAX_MESSAGE_LENGTH];
	SQLINTEGER NativeError;
	SQLSMALLINT i = 1, MsgLen;

	while (SQLGetDiagRec(SQL_HANDLE_STMT, lpStatement, i, SqlState, &NativeError, Msg, sizeof(Msg), &MsgLen) != SQL_NO_DATA)
	{
		Log << (char*)SqlState << (char*)Msg << string("\r\n");
		i++;
	}

	return (int)(i - 1);
}

bool database::odbc::recordset::Execute(string sql)
{
	return Execute(sql.c_str());
}

bool database::odbc::recordset::Execute(const char *sql)
{
	if (lpStatement == NULL) return false;
	SQLRETURN result = SQLExecDirect(lpStatement, (SQLCHAR*)sql, SQL_NTS);
	return ((result == SQL_SUCCESS) || (result == SQL_SUCCESS_WITH_INFO));
}

bool database::odbc::recordset::Prepare(string sql)
{
	return Prepare(sql.c_str());
}

bool database::odbc::recordset::Prepare(const char *sql)
{
	if (lpStatement == NULL) return false;
	SQLRETURN result = SQLPrepare(lpStatement, (SQLCHAR*)sql, (SQLINTEGER)strlen(sql));
	return result != SQL_ERROR;
}

void database::odbc::recordset::cleanup()
{
	if (lpStatement != NULL)
	{
		try
		{
			SQLFreeHandle(SQL_HANDLE_STMT, lpStatement);
		}
		catch (...)
		{
			Log << string("odbc::recordset::cleanup SEH Error\r\n");
		}

		lpStatement = NULL;
	}
}

void database::odbc::connection::reset()
{
	isopen = false; init = false; cleanup();

	if (SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &lpEnvironment) == SQL_ERROR) return;
	if (SQLSetEnvAttr(lpEnvironment, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, 0) != SQL_SUCCESS) return;
	if (SQLAllocHandle(SQL_HANDLE_DBC, lpEnvironment, &lpConnection) != SQL_SUCCESS) return;

	SQLSetConnectAttr(lpConnection, SQL_LOGIN_TIMEOUT, (SQLPOINTER)120, 0);

	init = true;
}

bool database::odbc::connection::open(string connection)
{
	return open(connection.c_str());
}

bool database::odbc::connection::open(const char *connection)
{
	SQLCHAR out[255];
	SQLSMALLINT len;

	SQLRETURN result = SQLDriverConnect(lpConnection, NULL, (SQLCHAR*)connection, SQL_NTS, out, 255, &len, SQL_DRIVER_NOPROMPT);

	if (result == SQL_SUCCESS || result == SQL_SUCCESS_WITH_INFO)
	{
		isopen = true;
		return true;
	}
	else logConnectionError();

	return false;
}

bool database::odbc::connection::close()
{
	if (!isopen) return false;
	bool result = true;

	if (lpConnection != NULL)
	{
		SQLRETURN sqlret = SQLDisconnect(lpConnection);
		if ((result != SQL_SUCCESS) && (result != SQL_SUCCESS_WITH_INFO)) result = false;
		if (SQLFreeHandle(SQL_HANDLE_DBC, lpConnection) != SQL_SUCCESS) result = false;
	}

	if (lpEnvironment != NULL) if (SQLFreeHandle(SQL_HANDLE_ENV, lpEnvironment) != SQL_SUCCESS) result = false;

	isopen = false;
	return result;
}

bool database::odbc::connection::executeNoResults(string sql)
{
	return executeNoResults(sql.c_str());
}


bool database::odbc::connection::executeWithResults(string sql, database::recordset *result)
{
	return executeWithResults(sql.c_str(), result);
}

long database::odbc::connection::executeScalar(string sql)
{
	return executeScalar(sql.c_str());
}

bool database::odbc::connection::Prepare(string sql, database::recordset *result)
{
	return Prepare(sql.c_str(), result);
}

bool database::odbc::connection::executeNoResults(const char *sql)
{
	if (!isopen) return false;
	bool result = false;

	try
	{
		SQLHANDLE lpStatement = NULL;
		if (SQLAllocHandle(SQL_HANDLE_STMT, lpConnection, &lpStatement) == SQL_ERROR) return false;
		if (SQLExecDirect(lpStatement, (SQLCHAR*)sql, SQL_NTS) != SQL_ERROR) result = true;
		if (SQLFreeHandle(SQL_HANDLE_STMT, lpStatement) != SQL_SUCCESS) result = false;
	}
	catch (...) { return false; }

	return result;

}

bool database::odbc::connection::executeWithResults(const char *sql, database::recordset *result)
{
	database::odbc::recordset *temp = (database::odbc::recordset*)result;

	if (temp->create(lpConnection))
	{
		return temp->Execute(sql);
	}

	return false;
}

long database::odbc::connection::executeScalar(const char *sql)
{
	long result = -1L;
	if (!isopen) return result;

	try
	{
		SQLHANDLE lpStatement = NULL;
		if (SQLAllocHandle(SQL_HANDLE_STMT, lpConnection, &lpStatement) == SQL_ERROR) return result;
		if (SQLExecDirect(lpStatement, (SQLCHAR*)sql, SQL_NTS) != SQL_ERROR)//== SQL_SUCCESS)
		{
			if (SQLFetch(lpStatement) == SQL_SUCCESS) SQLGetData(lpStatement, 1, SQL_C_ULONG, &result, 0, NULL);
		}
		if (SQLFreeHandle(SQL_HANDLE_STMT, lpStatement) != SQL_SUCCESS) result = -1L;
	}
	catch (...) { return -1L; }

	return result;
}

bool database::odbc::connection::Prepare(const char *sql, database::recordset *result)
{
	database::odbc::recordset *temp = (database::odbc::recordset*)result;

	if (temp->create(lpConnection))
	{
		return temp->Prepare(sql);
	}

	return false;
}

int database::odbc::connection::logConnectionError()
{
	SQLCHAR SqlState[6], Msg[SQL_MAX_MESSAGE_LENGTH];
	SQLINTEGER NativeError;
	SQLSMALLINT i = 1, MsgLen;

	while (SQLGetDiagRec(SQL_HANDLE_DBC, lpConnection, i, SqlState, &NativeError, Msg, sizeof(Msg), &MsgLen) != SQL_NO_DATA)
	{
		Log << (char*)SqlState << (char*)Msg << string("\r\n");
		i++;
	}

	return (int)(i - 1);
}

void database::odbc::connection::makeNull()
{
	lpConnection = NULL;
	lpEnvironment = NULL;
}

void database::odbc::connection::cleanup()
{
	if (isopen) close();
}

/*
void database::odbc::factory::connection::reset()
{
	init = false; cleanup();

	init = true;
}
*/
/*
void database::odbc::factory::connection::makeNull()
{
}

void database::odbc::factory::connection::cleanup()
{
	#warning this function needs something?
}*/