#include "odbc.h"
#include "string.h"
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

bool database::odbc::recordset::Execute()
{
	if (lpStatement == NULL) return false;
	SQLRETURN result = SQLExecute(lpStatement);

	return ((result == SQL_SUCCESS) || (result == SQL_SUCCESS_WITH_INFO));
}

int database::odbc::recordset::logStatementError()
{
	SQLCHAR SqlState[6], Msg[SQL_MAX_MESSAGE_LENGTH];
	SQLINTEGER NativeError;
	SQLSMALLINT i = 1, MsgLen;

	while (SQLGetDiagRec(SQL_HANDLE_STMT, lpStatement, i, SqlState, &NativeError, Msg, sizeof(Msg), &MsgLen) != SQL_NO_DATA)
	{
		Log << (char*)SqlState << (char*)Msg << "\r\n";
		i++;
	}

	return (int)(i - 1);
}

bool database::odbc::recordset::Execute(string &sql)
{
	return Execute(sql.c_str());
}

bool database::odbc::recordset::Execute(const char *sql)
{
	if (lpStatement == NULL) return false;
	SQLRETURN result = SQLExecDirect(lpStatement, (SQLCHAR*)sql, SQL_NTS);
	return ((result == SQL_SUCCESS) || (result == SQL_SUCCESS_WITH_INFO));
}

bool database::odbc::recordset::Prepare(string &sql)
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
			Log << "database::recordset::cleanup SEH Error\r\n";
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

bool database::odbc::connection::open(string &connection)
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

bool database::odbc::connection::executeNoResults(string &sql)
{
	return executeNoResults(sql.c_str());
}


bool database::odbc::connection::executeWithResults(string &sql, database::recordset *result)
{
	return executeWithResults(sql.c_str(), result);
}

long database::odbc::connection::executeScalar(string &sql)
{
	return executeScalar(sql.c_str());
}

bool database::odbc::connection::Prepare(string &sql, database::recordset *result)
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
		Log << (char*)SqlState << (char*)Msg << "\r\n";
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

void database::odbc::factory::connection::reset()
{
	init = false; cleanup();

	init = true;
}

database::connection *database::odbc::factory::connection::get()
{
	database::odbc::connection *result = new database::odbc::connection();
	if(result != NULL) connections.push_back(result);
	return result;
}

void database::odbc::factory::connection::makeNull()
{
}

void database::odbc::factory::connection::cleanup()
{
	for (long i = (long)connections.size() - 1L; i >= 0L; i--)
	{
		database::odbc::connection *temp = connections[i];
		if (temp != NULL) delete temp;
	}
}

void database::odbc::factory::recordset::reset()
{
	init = false; cleanup();

	init = true;
}

database::recordset *database::odbc::factory::recordset::get()
{
	database::odbc::recordset *result = new database::odbc::recordset();
	if (result != NULL) recordsets.push_back(result);
	return result;
}

void database::odbc::factory::recordset::makeNull()
{
}

void database::odbc::factory::recordset::cleanup()
{
	for (long i = (long)recordsets.size() - 1L; i >= 0L; i--)
	{
		database::odbc::recordset *temp = recordsets[i];
		if (temp != NULL) delete temp;
	}
}