#include "records.h"
#include "sqltypes.h"

bool database::records::message::bind(database::recordset *recordset)
{
	if (!recordset->BindLong(1L, messageID)) return false;
	if (!recordset->BindString(2L, (SQLCHAR*)user)) return false;
	if (!recordset->BindString(3L, (SQLCHAR*)APIKey)) return false;
	if (!recordset->BindString(4L, (SQLCHAR*)GUID)) return false;
	if (!recordset->BindDateTime(5L, created)) return false;
	if (!recordset->BindDateTime(6L, finished)) return false;

	return true;
}

void database::records::message::set(data::message::message &source)
{
	messageID = source.messageID;
	source.user.toChar(user, MAX);
	source.APIkey.toChar(APIKey, MAX);
	source.GUID.toChar(APIKey, MAX);
	created = (TIMESTAMP_STRUCT)source.created;
	finished = (TIMESTAMP_STRUCT)source.finished;
}

bool database::records::item::bind(database::recordset *recordset)
{
	if (!recordset->BindLong(1L, itemID)) return false;
	if (!recordset->BindLong(2L, messageID)) return false;
	if (!recordset->BindString(3L, (SQLCHAR*)name)) return false;

	return true;
}

void database::records::item::set(data::item::item &source)
{
	itemID = source.itemID;
	messageID = source.messageID;
	source.name.toChar(name, MAX);
}

bool database::records::line::bind(database::recordset *recordset)
{
	if (!recordset->BindLong(1L, lineID)) return false;
	if (!recordset->BindLong(2L, itemID)) return false;
	if (!recordset->BindDateTime(3L, start)) return false;
	if (!recordset->BindDateTime(4L, end)) return false;
	if (!recordset->BindLong(5L, exclusivityID)) return false;
	if (!recordset->BindLong(6L, typeID)) return false;

	return true;
}

void database::records::line::set(data::line::line &source)
{
	lineID = source.lineID;
	itemID = source.itemID;
	start = (TIMESTAMP_STRUCT)source.start;
	end = (TIMESTAMP_STRUCT)source.end;
	exclusivityID = source.exclusivityID;
	typeID = source.typeID;
}

bool database::records::component::line::component::bind(database::recordset *recordset)
{
	if (!recordset->BindLong(1L, componentID)) return false;
	if (!recordset->BindLong(2L, lineID)) return false;
	if (!recordset->BindString(3L, (SQLCHAR*)name)) return false;

	return true;
}

void database::records::component::line::component::set(data::component::line::component &source)
{
	componentID = source.componentID;
	lineID = source.lineID;
	source.name.toChar(name, MAX);
}

bool database::records::element::bind(database::recordset *recordset)
{
	if (!recordset->BindLong(1L, elementID)) return false;
	if (!recordset->BindLong(2L, componentID)) return false;
	if (!recordset->BindString(3L, (SQLCHAR*)value)) return false;

	return true;
}

void database::records::element::set(data::element::element &source)
{
	elementID = source.elementID;
	componentID = source.componentID;
	source.value.toChar(value, MAX);
}
