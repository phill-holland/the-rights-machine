#include "records.h"
#include "sqltypes.h"

bool database::records::message::bind(database::recordset *recordset)
{
	if (!recordset->BindString(1L, (SQLCHAR*)messageID)) return false;
	if (!recordset->BindString(2L, (SQLCHAR*)user)) return false;
	if (!recordset->BindString(3L, (SQLCHAR*)APIKey)) return false;
	if (!recordset->BindString(4L, (SQLCHAR*)GUID)) return false;
	if (!recordset->BindDateTime(5L, created)) return false;
	if (!recordset->BindDateTime(6L, finished)) return false;

	return true;
}

void database::records::message::set(data::message::message &source)
{
	source.user.toChar(user, database::records::message::MAX);
	source.APIkey.toChar(APIKey, database::records::message::MAX);
	source.GUID.toChar(GUID, database::records::message::MAX);
	created = source.created;
	finished = source.finished;
}

bool database::records::item::bind(database::recordset *recordset)
{
	if (!recordset->BindString(1L, (SQLCHAR*)itemID)) return false;
	if (!recordset->BindString(2L, (SQLCHAR*)messageID)) return false;
	if (!recordset->BindString(3L, (SQLCHAR*)name)) return false;

	return true;
}

void database::records::item::set(data::item::item &source)
{
	source.name.toChar(name, MAX);
}

bool database::records::line::bind(database::recordset *recordset)
{
	if (!recordset->BindString(1L, (SQLCHAR*)lineID)) return false;
	if (!recordset->BindString(2L, (SQLCHAR*)itemID)) return false;
	if (!recordset->BindDateTime(3L, start)) return false;
	if (!recordset->BindDateTime(4L, end)) return false;
	if (!recordset->BindLong(5L, exclusivityID)) return false;
	if (!recordset->BindLong(6L, typeID)) return false;

	return true;
}

void database::records::line::set(data::line::line &source)
{
	start = source.start;
	end = source.end;
	exclusivityID = source.exclusivityID;
	typeID = source.typeID;
}

bool database::records::component::line::component::bind(database::recordset *recordset)
{
	if (!recordset->BindString(1L, (SQLCHAR*)componentID)) return false;
	if (!recordset->BindString(2L, (SQLCHAR*)lineID)) return false;
	if (!recordset->BindString(3L, (SQLCHAR*)name)) return false;

	return true;
}

void database::records::component::query::component::set(data::component::query::component &source)
{
	source.name.toChar(name, MAX);
}

bool database::records::component::query::component::bind(database::recordset *recordset)
{
	if (!recordset->BindString(1L, (SQLCHAR*)componentID)) return false;
	if (!recordset->BindString(2L, (SQLCHAR*)queryID)) return false;
	if (!recordset->BindString(3L, (SQLCHAR*)name)) return false;

	return true;
}

void database::records::component::line::component::set(data::component::line::component &source)
{
	source.name.toChar(name, MAX);
}

bool database::records::element::bind(database::recordset *recordset)
{
	if (!recordset->BindString(1L, (SQLCHAR*)elementID)) return false;
	if (!recordset->BindString(2L, (SQLCHAR*)componentID)) return false;
	if (!recordset->BindString(3L, (SQLCHAR*)value)) return false;

	return true;
}

void database::records::element::set(data::element::element &source)
{
	source.value.toChar(value, MAX);
}
