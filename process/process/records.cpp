#include "records.h"
#include "sqltypes.h"

bool database::records::message::bind(database::recordset *recordset)
{
	if (!recordset->BindLong(1L, messageID)) return false;
	if (!recordset->BindString(2L, (SQLCHAR*)user)) return false;
	if (!recordset->BindString(3L, (SQLCHAR*)APIKey)) return false;
	if (!recordset->BindString(4L, (SQLCHAR*)GUID)) return false;

	return true;
}

void database::records::message::set(data::message::message &source)
{
	messageID = source.messageID;
	source.user.toChar(user, MAX);
	source.APIkey.toChar(APIKey, MAX);
	source.GUID.toChar(APIKey, MAX);
}
