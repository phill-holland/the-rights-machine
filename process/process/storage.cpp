#include "storage.h"

bool database::storage::element::open(database::settings &settings)
{
	connection = settings.getConnections()->get();
	if (connection == NULL) return false;
	if (!connection->open(settings.getLocation())) return false;

	recordset = settings.getRecordSets()->get();
	if (recordset == NULL)
	{
		connection->close();

		return false;
	}
	
	return true;
}

bool database::storage::element::close()
{
	recordset->close();
	if (!connection->close()) return false;

	return true;
}

bool database::storage::element::read(data::element::element &destination)
{
	string key = (string)guid::guid(componentID);

	std::unordered_map<string, std::vector<database::records::element>, hasher, equality>::iterator i = data.find(key);
	if (i == data.end()) if (!load()) return false;

	std::unordered_map<string, std::vector<database::records::element>, hasher, equality>::iterator j = data.find(key);
	if (j == data.end()) return false;

	std::vector<database::records::element> source = data[key];

	if (source.size() > 0)
	{
		database::records::element temp = data[key].back();
		data[key].pop_back();

		destination.value = string(temp.value);

		return true;
	}

	return false;
}

bool database::storage::element::write(data::element::element &source)
{
	bool prepared = true;

	if (!recordset->IsInitalised())
	{
		string sql = "INSERT INTO tElement (elementID, componentID, value)";
		sql.concat(" VALUES(?,?,?);");

		if (!connection->Prepare(sql, recordset)) prepared = false;
	}

	if ((recordset->IsInitalised()) && (prepared))
	{
		bound.set(source);

		GUID unique = this->generate();
		bound.componentID = componentID;
		bound.elementID = unique;

		bound.bind(recordset);

		if (!recordset->Execute()) return false;

		return true;
	}

	return false;
}

bool database::storage::element::load()
{
	clear();

	if (!recordset->IsInitalised())
	{
		string sql("SELECT elementID, componentID, value FROM tElement ");
		sql.concat("LEFT JOIN tComponent ON tElement.componentID = tComponent.componentID ");
		sql.concat("LEFT JOIN tLine ON tComponent.lineID = tLine.lineID ");
		sql.concat("LEFT jOIN tItem ON tLine.itemID = tItem.itemID ");
		sql.concat("WHERE 1=1");
		
		if (identities.size() > 0UL)
		{
			sql.concat(" AND (");
			for (unsigned long i = 0UL; i < identities.size(); ++i)
			{
				string temp = identities[i];
				if (i > 0UL) sql.concat(" OR ");
				sql.concat("tItem.messageID='");
				sql.concat(temp);
				sql.concat("'");
			}
			sql.concat(")");
		}

		sql.concat(string(";"));

		if (connection->executeWithResults(sql, recordset))
		{
			while (recordset->MoveNext())
			{
				database::records::element temp;

				temp.elementID = recordset->GetGUID(1L);
				temp.componentID = recordset->GetGUID(2L);
				recordset->GetString(3L).toChar(temp.value, database::records::element::MAX);

				data[(string)guid::guid(componentID)].push_back(temp);
			};
		}
		else return false;
	}
	else return false;

	return (data.size() > 0);
}

bool database::storage::component::open(database::settings &settings)
{
	connection = settings.getConnections()->get();
	if (connection == NULL) return false;
	if (!connection->open(settings.getLocation())) return false;

	recordset = settings.getRecordSets()->get();
	if (recordset == NULL)
	{
		connection->close();

		return false;
	}

	if (!element.open(settings))
	{
		recordset->close();
		connection->close();

		return false;
	}

	return true;
}

bool database::storage::component::close()
{
	int index = 0;

	recordset->close();

	if (!element.close()) ++index;
	if (!connection->close()) ++index;

	return index == 0;
}

bool database::storage::component::read(data::component::line::component &destination)
{
	string key = (string)guid::guid(lineID);

	std::unordered_map<string, std::vector<database::records::component::line::component>, hasher, equality>::iterator i = data.find(key);
	if (i == data.end())
	{
		if (!load()) return false;

		element.identities = identities;
	}

	std::unordered_map<string, std::vector<database::records::component::line::component>, hasher, equality>::iterator j = data.find(key);
	if (j == data.end()) return false;

	std::vector<database::records::component::line::component> source = data[key];

	if (source.size() > 0)
	{
		database::records::component::line::component temp = data[key].back();
		data[key].pop_back();

		destination.name = (string)temp.name;

		if (parent != NULL)
		{
			element.componentID = temp.componentID;
			element.parent = parent;
			if (!parent->load(&element)) return false;
		}

		return true;
	}

	return false;
}

bool database::storage::component::write(data::component::line::component &source)
{
	bool prepared = true;

	if (!recordset->IsInitalised())
	{
		string sql = "INSERT INTO tComponent (componentID, lineID, name)";
		sql.concat(" VALUES(?,?,?);");

		if (!connection->Prepare(sql, recordset)) prepared = false;
	}

	if ((recordset->IsInitalised()) && (prepared))
	{
		bound.set(source);

		GUID unique = this->generate();
		bound.lineID = lineID;
		bound.componentID = unique;

		bound.bind(recordset);

		if (!recordset->Execute()) return false;

		if (parent != NULL)
		{
			element.componentID = unique;
			element.parent = parent;
			if (!parent->save(&element)) return false;
		}

		return true;
	}

	return false;
}

bool database::storage::component::load()
{
	clear();

	if (!recordset->IsInitalised())
	{
		string sql("SELECT componentID, lineID, name FROM tComponent ");
		sql.concat("LEFT JOIN tLine ON tComponent.lineID = tLine.lineID ");
		sql.concat("LEFT jOIN tItem ON tLine.itemID = tItem.itemID ");
		sql.concat("WHERE 1=1");

		if (identities.size() > 0UL)
		{
			sql.concat(" AND (");
			for (unsigned long i = 0UL; i < identities.size(); ++i)
			{
				string temp = identities[i];
				if (i > 0UL) sql.concat(" OR ");
				sql.concat("tItem.messageID='");
				sql.concat(temp);
				sql.concat("'");
			}
			sql.concat(")");
		}

		sql.concat(string(";"));

		if (connection->executeWithResults(sql, recordset))
		{
			while (recordset->MoveNext())
			{
				database::records::component::line::component temp;

				temp.componentID = recordset->GetGUID(1L);
				temp.lineID = recordset->GetGUID(2L);
				recordset->GetString(3L).toChar(temp.name, database::records::component::line::component::MAX);

				data[(string)guid::guid(lineID)].push_back(temp);
			};
		}
		else return false;
	}
	else return false;

	return (data.size() > 0);
}

bool database::storage::line::open(database::settings &settings)
{
	connection = settings.getConnections()->get();
	if (connection == NULL) return false;
	if (!connection->open(settings.getLocation())) return false;

	recordset = settings.getRecordSets()->get();
	if (recordset == NULL)
	{
		connection->close();

		return false;
	}

	if (!component.open(settings))
	{
		recordset->close();
		connection->close();

		return false;
	}

	return true;
}

bool database::storage::line::close()
{
	component.close();

	if(recordset != NULL) recordset->close();
	if (connection != NULL) connection->close();

	return true;
}

bool database::storage::line::read(data::line::line &destination)
{
	string key = (string)guid::guid(itemID);

	std::unordered_map<string, std::vector<records::line>, hasher, equality>::iterator i = data.find(key);
	if (i == data.end())
	{
		if (!load()) return false;

		component.identities = identities;
	}

	std::unordered_map<string, std::vector<records::line>, hasher, equality>::iterator j = data.find(key);
	if (j == data.end()) return false;

	std::vector<records::line> source = data[key];

	if (source.size() > 0)
	{
		records::line temp = data[key].back();
		data[key].pop_back();

		destination.start = (datetime)temp.start;
		destination.end = (datetime)temp.end;
		destination.exclusivityID = temp.exclusivityID;
		destination.typeID = temp.typeID;

		if (parent != NULL)
		{
			component.lineID = temp.lineID;
			component.parent = parent;
			if (!parent->load(&component)) return false;
		}

		return true;
	}

	return false;
}

bool database::storage::line::write(data::line::line &source)
{
	bool prepared = true;

	if (!recordset->IsInitalised())
	{
		string sql = "INSERT INTO tLine (lineID, itemID, startDate, endDate, exclusivityID, typeID)";
		sql.concat(" VALUES(?,?,?,?,?,?);");

		if (!connection->Prepare(sql, recordset)) prepared = false;
	}

	if ((recordset->IsInitalised()) && (prepared))
	{
		bound.set(source);

		GUID unique = this->generate();
		bound.itemID = itemID;
		bound.lineID = unique;

		bound.bind(recordset);

		if (!recordset->Execute()) return false;

		if (parent != NULL)
		{
			component.lineID = unique;
			component.parent = parent;
			if (!parent->save(&component)) return false;
		}

		return true;
	}

	return false;
}

bool database::storage::line::load()
{
	clear();

	if (!recordset->IsInitalised())
	{
		string sql("SELECT lineID, itemID, startDate, endDate, exclusivityID, typeID FROM tLine ");
		sql.concat(" LEFT JOIN tItem ON tLine.itemID = tItem.itemID WHERE 1=1");

		if (identities.size() > 0UL)
		{
			sql.concat(" AND (");
			for (unsigned long i = 0UL; i < identities.size(); ++i)
			{
				string temp = identities[i];
				if (i > 0UL) sql.concat(" OR ");
				sql.concat("tItem.messageID='");
				sql.concat(temp);
				sql.concat("'");
			}
			sql.concat(")");
		}

		sql.concat(string(";"));

		if (connection->executeWithResults(sql, recordset))
		{
			while (recordset->MoveNext())
			{
				records::line temp;

				temp.lineID = recordset->GetGUID(1L);
				temp.itemID = recordset->GetGUID(2L);
				temp.start = recordset->GetTimeStamp(3L);
				temp.end = recordset->GetTimeStamp(4L);
				temp.exclusivityID = recordset->GetLong(5L);
				temp.typeID = recordset->GetLong(6L);

				data[(string)guid::guid(itemID)].push_back(temp);
			};
		}
		else return false;
	}
	else return false;

	return (data.size() > 0);
}

bool database::storage::item::open(database::settings &settings)
{
	connection = settings.getConnections()->get();
	if (connection == NULL) return false;
	if (!connection->open(settings.getLocation())) return false;

	recordset = settings.getRecordSets()->get();
	if (recordset == NULL)
	{
		connection->close();
		
		return false;
	}

	if (!line.open(settings))
	{
		recordset->close();
		connection->close();

		return false;
	}

	return true;
}

bool database::storage::item::close()
{
	int index = 0;

	recordset->close();

	if (!line.close()) ++index;
	if (!connection->close()) ++index;

	return index == 0;
}

bool database::storage::item::read(data::item::item &destination)
{
	string key = (string)guid::guid(messageID);

	std::unordered_map<string, std::vector<records::item>, hasher, equality>::iterator i = data.find(key);
	if (i == data.end())
	{
		if (!load()) return false;

		line.identities = identities;
	}

	std::unordered_map<string, std::vector<records::item>, hasher, equality>::iterator j = data.find(key);
	if (j == data.end()) return false;

	std::vector<records::item> source = data[key];
	if (source.size() > 0)
	{
		records::item temp = data[key].back();
		data[key].pop_back();

		destination.name = (string)temp.name;

		if (parent != NULL)
		{
			line.itemID = temp.itemID;
			line.parent = parent;
			if (!parent->load(&line)) return false;
		}

		return true;
	}

	return false;
}

bool database::storage::item::write(data::item::item &source)
{
	bool prepared = true;

	if (!recordset->IsInitalised())
	{
		string sql = "INSERT INTO tItem (itemID, messageID, name)";
		sql.concat(" VALUES(?,?,?);");

		if (!connection->Prepare(sql, recordset)) prepared = false;
	}

	if ((recordset->IsInitalised()) && (prepared))
	{
		bound.set(source);

		GUID unique = this->generate();
		bound.messageID = messageID;
		bound.itemID = unique;

		bound.bind(recordset);

		if (!recordset->Execute()) return false;

		if (parent != NULL)
		{
			line.itemID = unique;
			line.parent = parent;
			if (!parent->save(&line)) return false;
		}

		return true;
	}

	return false;
}

bool database::storage::item::load()
{
	clear();

	if (!recordset->IsInitalised())
	{
		string sql("SELECT itemID, messageID, name FROM tItem WHERE 1=1");
		
		if (identities.size() > 0UL)
		{
			sql.concat(" AND (");
			for (unsigned long i = 0UL; i < identities.size(); ++i)
			{
				string temp = identities[i];
				if (i > 0UL) sql.concat(" OR ");
				sql.concat("messageID='");
				sql.concat(temp);
				sql.concat("'");
			}
			sql.concat(")");
		}

		sql.concat(string(";"));

		if (connection->executeWithResults(sql, recordset))
		{
			while (recordset->MoveNext())
			{
				records::item temp;
				
				temp.itemID = recordset->GetGUID(1L);
				temp.messageID = recordset->GetGUID(2L);
				recordset->GetString(3L).toChar(temp.name, records::item::MAX);

				data[(string)guid::guid(temp.messageID)].push_back(temp);
			};
		}
		else return false;
	}
	else return false;

	return (data.size() > 0);
}

bool database::storage::message::open(database::settings &settings)
{
	connection = settings.getConnections()->get();
	if (connection == NULL) return false;
	if (!connection->open(settings.getLocation())) return false;

	recordset = settings.getRecordSets()->get();
	if (recordset == NULL) 
	{ 
		connection->close(); 

		return false; 
	}

	if (!item.open(settings))
	{
		recordset->close();
		connection->close();

		return false;
	}

	return true;
}

bool database::storage::message::close()
{
	int index = 0;

	recordset->close();

	if (!item.close()) ++index;
	if (!connection->close()) ++index;

	return index == 0;
}

bool database::storage::message::read(data::message::message &destination)
{
	if (data.size() == 0)
	{
		if (!load()) return false;

		item.identities = identities;		
	}

	if (data.size() > 0)
	{		
		records::message temp = data.back();
		data.pop_back();

		destination.user = (string)guid::guid(temp.user);
		destination.guid = (string)guid::guid(temp.guid);
		destination.apikey = (string)guid::guid(temp.apikey);
		destination.created = (datetime)temp.created;
		destination.finished = (datetime)temp.finished;
		
		item.messageID = temp.messageID;		
		item.parent = &destination;
		destination.load(&item);

		return true;
	}

	return false;
}

bool database::storage::message::write(data::message::message &source)
{
	bool prepared = true;

	if (!recordset->IsInitalised())
	{
		string sql = "INSERT INTO tMessage (messageID, [user], [GUID], APIKey, created, finished)";
		sql.concat(" VALUES(?,?,?,?,?,?);");

		if (!connection->Prepare(sql, recordset)) prepared = false;
	}


	if ((recordset->IsInitalised()) && (prepared))
	{
		bound.set(source);

		GUID unique = this->generate();
		bound.messageID = unique;

		bound.bind(recordset);

		if (!recordset->Execute()) return false;

		item.messageID = unique;
		item.parent = &source;
		return source.save(&item);
	}

	return false;
}

bool database::storage::message::load()
{	
	clear();

	if (!recordset->IsInitalised())
	{
		string order = "";

		string sql("SELECT");
		if (max > 0L)
		{
			sql.concat(" TOP ");
			sql.concat(string::fromLong(max));

			order = " ORDER BY created DESC";
		}

		sql.concat(" messageID, [User], [GUID], APIKey, Created, Finished FROM tMessage WHERE 1=1");
		if (order.length() > 0) sql.concat(order);

		sql.concat(string(";"));

		if (connection->executeWithResults(sql, recordset))
		{
			while (recordset->MoveNext())
			{
				records::message temp;

				temp.messageID = recordset->GetGUID(1L);
				temp.user = recordset->GetGUID(2L);
				temp.guid = recordset->GetGUID(3L);
				temp.apikey = recordset->GetGUID(4L);
				temp.created = recordset->GetTimeStamp(5L);
				temp.finished = recordset->GetTimeStamp(6L);

				data.push_back(temp);
				identities.push_back((string)guid::guid(temp.messageID));
			};
		}
		else return false;
	}
	else return false;

	return (data.size() > 0);
}