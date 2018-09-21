#include "records.h"
#include "sqltypes.h"
#include "guid.h"

void database::records::message::clear()
{
	memset(&messageID, 0, sizeof(GUID));
	memset(&user, 0, sizeof(GUID));
	memset(&apikey, 0, sizeof(GUID));
	memset(&guid, 0, sizeof(GUID));
	memset(&created, 0, sizeof(TIMESTAMP_STRUCT));
	memset(&finished, 0, sizeof(TIMESTAMP_STRUCT));
}

bool database::records::message::bind(database::recordset *recordset)
{
	if (!recordset->BindGUID(1L, messageID)) return false;
	if (!recordset->BindGUID(2L, user)) return false;
	if (!recordset->BindGUID(3L, apikey)) return false;
	if (!recordset->BindGUID(4L, guid)) return false;
	if (!recordset->BindTimeStamp(5L, created)) return false;
	if (!recordset->BindTimeStamp(6L, finished)) return false;

	return true;
}

void database::records::message::set(data::message::message &source)
{
	clear();

	user = (GUID)guid::guid(source.user);
	apikey = (GUID)guid::guid(source.apikey);
	guid = (GUID)guid::guid(source.guid);
	created = (TIMESTAMP_STRUCT)source.created;
	finished = (TIMESTAMP_STRUCT)source.finished;
}

void database::records::item::clear()
{	
	memset(&itemID, 0, sizeof(GUID));
	memset(&messageID, 0, sizeof(GUID));
	memset(name, 0, MAX);
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
	clear();

	source.name.toChar(name, MAX);
}

void database::records::line::clear()
{
	memset(&lineID, 0, sizeof(GUID));
	memset(&itemID, 0, sizeof(GUID));
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

void database::records::line::set(data::line::line &source)
{
	clear();

	start = (TIMESTAMP_STRUCT)source.start;
	end = (TIMESTAMP_STRUCT)source.end;
	exclusivityID = source.exclusivityID;
	typeID = source.typeID;
}

void database::records::component::line::component::clear()
{
	memset(&componentID, 0, sizeof(GUID));
	memset(&lineID, 0, sizeof(GUID));
	memset(name, 0, MAX);
}

void database::records::component::line::component::set(data::component::line::component &source)
{
	clear();

	source.name.toChar(name, MAX);
}

bool database::records::component::line::component::bind(database::recordset *recordset)
{
	if (!recordset->BindGUID(1L, componentID)) return false;
	if (!recordset->BindGUID(2L, lineID)) return false;
	if (!recordset->BindString(3L, (SQLCHAR*)name)) return false;

	return true;
}

void database::records::component::query::component::clear()
{
	memset(&componentID, 0, sizeof(GUID));
	memset(&queryID, 0, sizeof(GUID));
	memset(name, 0, MAX);
}

void database::records::component::query::component::set(data::component::query::component &source)
{
	clear();

	source.name.toChar(name, MAX);
}

bool database::records::component::query::component::bind(database::recordset *recordset)
{
	if (!recordset->BindGUID(1L, componentID)) return false;
	if (!recordset->BindGUID(2L, queryID)) return false;
	if (!recordset->BindString(3L, (SQLCHAR*)name)) return false;

	return true;
}

void database::records::element::clear()
{
	memset(&elementID, 0, sizeof(GUID));
	memset(&componentID, 0, sizeof(GUID));	
	memset(value, 0, MAX);
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
	clear();

	source.value.toChar(value, MAX);
}
