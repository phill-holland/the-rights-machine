#include "database/records.h"
#include "types/guid.h"
#include "sqltypes.h"
#include <cstring>

void database::records::message::clear()
{
	messageID.clear();
	user.clear();
	apikey.clear();
	guid.clear();
	tag.clear();
	memset(&created, 0, sizeof(TIMESTAMP_STRUCT));
}

bool database::records::message::bind(database::recordset *recordset)
{
	if (!recordset->BindGUID(1L, messageID)) return false;
	if (!recordset->BindGUID(2L, user)) return false;
	if (!recordset->BindGUID(3L, apikey)) return false;
	if (!recordset->BindGUID(4L, guid)) return false;
	if (!recordset->BindGUID(5L, tag)) return false;
	if (!recordset->BindTimeStamp(6L, created)) return false;

	return true;
}

void database::records::message::set(data::message::message &source)
{
	clear();

	guid = guid::guid(source.guid);
	created = (TIMESTAMP_STRUCT)source.created;
}

void database::records::item::clear()
{	
	itemID.clear();
	messageID.clear();

	memset(name, 0, MAX);
}

bool database::records::item::bind(database::recordset *recordset)
{
	if (!recordset->BindGUID(1L, itemID)) return false;
	if (!recordset->BindGUID(2L, messageID)) return false;
	if (!recordset->BindString(3L, (SQLCHAR*)name)) return false;

	return true;
}

void database::records::item::set(models::item::item &source)
{
	clear();

	source.name.toChar(name, MAX);
}

void database::records::query::clear()
{
	queryID.clear();
	messageID.clear();
}

bool database::records::query::bind(database::recordset * recordset)
{
	if (!recordset->BindGUID(1L, queryID)) return false;
	if (!recordset->BindGUID(2L, messageID)) return false;

	return true;
}

void database::records::query::set(models::query::query &source)
{
	clear();
}

void database::records::line::clear()
{
	lineID.clear();
	itemID.clear();

	memset(&start, 0, sizeof(TIMESTAMP_STRUCT));
	memset(&end, 0, sizeof(TIMESTAMP_STRUCT));

	exclusivityID = 0;
	typeID = 0;
}

bool database::records::line::bind(database::recordset *recordset)
{
	if (!recordset->BindGUID(1L, lineID)) return false;
	if (!recordset->BindGUID(2L, itemID)) return false;
	if (!recordset->BindTimeStamp(3L, start)) return false;
	if (!recordset->BindTimeStamp(4L, end)) return false;
	if (!recordset->BindLong(5L, exclusivityID)) return false;
	if (!recordset->BindLong(6L, typeID)) return false;

	return true;
}

void database::records::line::set(models::line::line &source)
{
	clear();

	start = (TIMESTAMP_STRUCT)source.start;
	end = (TIMESTAMP_STRUCT)source.end;
	exclusivityID = source.exclusivityID;
	typeID = source.typeID;
}

void database::records::component::line::component::clear()
{
	componentID.clear();
	lineID.clear();

	memset(name, 0, MAX);

	type = TYPE;
}

void database::records::component::line::component::set(models::component::line::component &source)
{
	clear();

	source.name.toChar(name, MAX);
}

bool database::records::component::line::component::bind(database::recordset *recordset)
{
	if (!recordset->BindGUID(1L, componentID)) return false;
	if (!recordset->BindGUID(2L, lineID)) return false;
	if (!recordset->BindLong(3L, type)) return false;
	if (!recordset->BindString(4L, (SQLCHAR*)name)) return false;

	return true;
}

void database::records::component::query::component::clear()
{
	componentID.clear();
	queryID.clear();

	memset(name, 0, MAX);

	type = TYPE;
}

void database::records::component::query::component::set(models::component::query::component &source)
{
	clear();

	source.name.toChar(name, MAX);
}

bool database::records::component::query::component::bind(database::recordset *recordset)
{
	if (!recordset->BindGUID(1L, componentID)) return false;
	if (!recordset->BindGUID(2L, queryID)) return false;
	if (!recordset->BindLong(3L, type)) return false;
	if (!recordset->BindString(4L, (SQLCHAR*)name)) return false;

	return true;
}

void database::records::element::clear()
{
	elementID.clear();
	componentID.clear();

	memset(value, 0, MAX);
}

bool database::records::element::bind(database::recordset *recordset)
{
	if (!recordset->BindGUID(1L, elementID)) return false;
	if (!recordset->BindGUID(2L, componentID)) return false;
	if (!recordset->BindString(3L, (SQLCHAR*)value)) return false;

	return true;
}

void database::records::element::set(models::element::element &source)
{
	clear();

	source.value.toChar(value, MAX);
}

void database::records::request::clear()
{
	requestID.clear();
	guid.clear();
	user.clear();
	tag.clear();
	memset(&created, 0, sizeof(TIMESTAMP_STRUCT));
}

bool database::records::request::bind(database::recordset *recordset)
{
	if (!recordset->BindGUID(1L, requestID)) return false;
	if (!recordset->BindGUID(2L, guid)) return false;
	if (!recordset->BindGUID(3L, user)) return false;
	if (!recordset->BindGUID(4L, tag)) return false;
	if (!recordset->BindTimeStamp(5L, created)) return false;

	return true;
}

void database::records::request::set(models::request::request &source)
{
	clear();

	user = guid::guid(source.user);
	guid = guid::guid(source.guid);
	created = (TIMESTAMP_STRUCT)source.created;
}

void database::records::response::clear()
{
	responseID.clear();
	guid.clear();
	user.clear();
	memset(&created, 0, sizeof(TIMESTAMP_STRUCT));

	status = 1L;
	available = false;	
}

bool database::records::response::bind(database::recordset *recordset)
{
	if (!recordset->BindGUID(1L, responseID)) return false;
	if (!recordset->BindGUID(2L, guid)) return false;
	if (!recordset->BindGUID(3L, user)) return false;
	if (!recordset->BindLong(4L, status)) return false;
	if (!recordset->BindTimeStamp(5L, created)) return false;
	if (!recordset->BindBool(6L, available)) return false;

	return true;
}

void database::records::response::set(models::response::response &source)
{
	clear();

	guid = guid::guid(source.guid);
	user = guid::guid(source.user);	
	status = source.status;
	created = (TIMESTAMP_STRUCT)source.created;
	available = source.available;
}
