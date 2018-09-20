#include "records.h"
#include "sqltypes.h"
#include "guid.h"

bool database::records::message::bind(database::recordset *recordset)
{
	if (!recordset->BindGUID(1L, messageID)) return false;
	if (!recordset->BindGUID(2L, user)) return false;
	if (!recordset->BindGUID(3L, apikey)) return false;
	if (!recordset->BindGUID(4L, guid)) return false;
	if (!recordset->BindString(5L, (SQLCHAR*)created)) return false;
	if (!recordset->BindString(6L, (SQLCHAR*)finished)) return false;

	return true;
}

void database::records::message::set(data::message::message &source)
{
	user = (GUID)guid::guid(source.user);
	apikey = (GUID)guid::guid(source.apikey);
	guid = (GUID)guid::guid(source.guid);
	((string)source.created).toChar(created, database::records::message::MAX);
	((string)source.finished).toChar(finished, database::records::message::MAX);
}

bool database::records::item::bind(database::recordset *recordset)
{
	if (!recordset->BindGUID(1L, itemID)) return false;
	if (!recordset->BindGUID(2L, messageID)) return false;
	if (!recordset->BindString(3L, (SQLCHAR*)name)) return false;

	return true;
}

void database::records::item::set(data::item::item &source)
{
	source.name.toChar(name, MAX);
}

bool database::records::line::bind(database::recordset *recordset)
{
	if (!recordset->BindGUID(1L, lineID)) return false;
	if (!recordset->BindGUID(2L, itemID)) return false;
	if (!recordset->BindString(3L, (SQLCHAR*)start)) return false;
	if (!recordset->BindString(4L, (SQLCHAR*)end)) return false;
	if (!recordset->BindLong(5L, exclusivityID)) return false;
	if (!recordset->BindLong(6L, typeID)) return false;

	return true;
}

void database::records::line::set(data::line::line &source)
{
	((string)source.start).toChar(start, database::records::message::MAX);
	((string)source.end).toChar(end, database::records::message::MAX);
	exclusivityID = source.exclusivityID;
	typeID = source.typeID;
}

bool database::records::component::line::component::bind(database::recordset *recordset)
{
	if (!recordset->BindGUID(1L, componentID)) return false;
	if (!recordset->BindGUID(2L, lineID)) return false;
	if (!recordset->BindString(3L, (SQLCHAR*)name)) return false;

	return true;
}

void database::records::component::query::component::set(data::component::query::component &source)
{
	source.name.toChar(name, MAX);
}

bool database::records::component::query::component::bind(database::recordset *recordset)
{
	if (!recordset->BindGUID(1L, componentID)) return false;
	if (!recordset->BindGUID(2L, queryID)) return false;
	if (!recordset->BindString(3L, (SQLCHAR*)name)) return false;

	return true;
}

void database::records::component::line::component::set(data::component::line::component &source)
{
	source.name.toChar(name, MAX);
}

bool database::records::element::bind(database::recordset *recordset)
{
	if (!recordset->BindGUID(1L, elementID)) return false;
	if (!recordset->BindGUID(2L, componentID)) return false;
	if (!recordset->BindString(3L, (SQLCHAR*)value)) return false;

	return true;
}

void database::records::element::set(data::element::element &source)
{
	source.value.toChar(value, MAX);
}
