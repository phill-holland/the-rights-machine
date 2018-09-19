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
	std::unordered_map<string, std::vector<database::records::element>, hasher, equality>::iterator i = data.find(componentID);
	if (i == data.end()) if (!load()) return false;

	std::unordered_map<string, std::vector<database::records::element>, hasher, equality>::iterator j = data.find(componentID);
	if (j == data.end()) return false;

	std::vector<database::records::element> source = data[componentID];

	if (source.size() > 0)
	{
		database::records::element temp = data[componentID].back();
		data[componentID].pop_back();

		destination.value = string(temp.value);

		return true;
	}

	return false;
}

bool database::storage::element::write(data::element::element &source)
{
	if (!recordset->IsInitalised())
	{
		string sql = "INSERT INTO tElement (elementID, componentID, value)";
		sql.concat(" VALUES(?,?,?);");

		connection->Prepare(sql, recordset);
	}

	if (recordset->IsInitalised())
	{
		bound.set(source);

		string unique = this->generate();
		unique.toChar(bound.elementID, database::records::element::MAX);

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

				recordset->GetString(1L).toChar(temp.elementID, database::records::element::MAX);

				string componentID = recordset->GetString(2L);
				componentID.toChar(temp.componentID, records::line::MAX);

				recordset->GetString(3L).toChar(temp.value, database::records::element::MAX);

				data[componentID].push_back(temp);
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
	std::unordered_map<string, std::vector<database::records::component::line::component>, hasher, equality>::iterator i = data.find(lineID);
	if (i == data.end())
	{
		if (!load()) return false;

		element.identities = identities;
	}

	std::unordered_map<string, std::vector<database::records::component::line::component>, hasher, equality>::iterator j = data.find(lineID);
	if (j == data.end()) return false;

	std::vector<database::records::component::line::component> source = data[lineID];

	if (source.size() > 0)
	{
		database::records::component::line::component temp = data[lineID].back();
		data[lineID].pop_back();

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
	if (!recordset->IsInitalised())
	{
		string sql = "INSERT INTO tComponent (componentID, lineID, name)";
		sql.concat(" VALUES(?,?,?);");

		connection->Prepare(sql, recordset);
	}

	if (recordset->IsInitalised())
	{
		bound.set(source);

		string unique = this->generate();
		unique.toChar(bound.componentID, database::records::component::line::component::MAX);

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

				recordset->GetString(1L).toChar(temp.componentID, database::records::component::line::component::MAX);
				
				string lineID = recordset->GetString(2L);
				lineID.toChar(temp.lineID, records::line::MAX);
				
				recordset->GetString(3L).toChar(temp.name, database::records::component::line::component::MAX);

				data[lineID].push_back(temp);
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
	std::unordered_map<string, std::vector<records::line>, hasher, equality>::iterator i = data.find(itemID);
	if (i == data.end())
	{
		if (!load()) return false;

		component.identities = identities;
	}

	std::unordered_map<string, std::vector<records::line>, hasher, equality>::iterator j = data.find(itemID);
	if (j == data.end()) return false;

	std::vector<records::line> source = data[itemID];

	if (source.size() > 0)
	{
		records::line temp = data[itemID].back();
		data[itemID].pop_back();

		destination.start = temp.start;
		destination.end = temp.end;
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
	if (!recordset->IsInitalised())
	{
		string sql = "INSERT INTO tLine (lineID, itemID, start, end, exclusivityID, typeID)";
		sql.concat(" VALUES(?,?,?,?,?,?);");

		connection->Prepare(sql, recordset);
	}

	if (recordset->IsInitalised())
	{
		bound.set(source);

		string unique = this->generate();
		unique.toChar(bound.lineID, database::records::line::MAX);

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
		string sql("SELECT lineID, itemID, start, end, exclusivityID, typeID FROM tLine ");
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

				recordset->GetString(1L).toChar(temp.lineID, records::line::MAX);

				string itemID = recordset->GetString(2L);
				itemID.toChar(temp.itemID, records::line::MAX);
							
				temp.start = recordset->GetDateTime(3L);
				temp.end = recordset->GetDateTime(4L);
				temp.exclusivityID = recordset->GetLong(5L);
				temp.typeID = recordset->GetLong(6L);

				data[itemID].push_back(temp);
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
	std::unordered_map<string, std::vector<records::item>, hasher, equality>::iterator i = data.find(messageID);
	if (i == data.end())
	{
		if (!load()) return false;

		line.identities = identities;
	}

	std::unordered_map<string, std::vector<records::item>, hasher, equality>::iterator j = data.find(messageID);
	if (j == data.end()) return false;

	std::vector<records::item> source = data[messageID];
	if (source.size() > 0)
	{
		records::item temp = data[messageID].back();
		data[messageID].pop_back();

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
	if (!recordset->IsInitalised())
	{
		string sql = "INSERT INTO tItem (itemID, messageID, name)";
		sql.concat(" VALUES(?,?,?);");

		connection->Prepare(sql, recordset);
	}

	if (recordset->IsInitalised())
	{
		bound.set(source);

		string unique = this->generate();
		unique.toChar(bound.itemID, database::records::item::MAX);

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

				string messageID = recordset->GetString(1L);
				messageID.toChar(temp.messageID, records::message::MAX);

				recordset->GetString(2L).toChar(temp.messageID, records::message::MAX);
				recordset->GetString(3L).toChar(temp.name, records::message::MAX);

				data[messageID].push_back(temp);
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

		destination.user = (string)temp.user;
		destination.GUID = (string)temp.GUID;
		destination.APIkey = (string)temp.APIKey;
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
	if (!recordset->IsInitalised())
	{
		string sql = "INSERT INTO tMessage (messageID, user, GUID, APIKEy, created, finished)";
		sql.concat(" VALUES(?,?,?,?,?,?);");

		connection->Prepare(sql, recordset);
	}

	if (recordset->IsInitalised())
	{
		bound.set(source);
		
		string unique = this->generate();
		unique.toChar(bound.messageID, database::records::message::MAX);

		bound.bind(recordset);

		if (!recordset->Execute()) return false;

		item.messageID = unique;
		item.parent = &source;
		source.save(&item);

		return true;
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

		sql.concat(" messageID, User, GUID, APIKey, Created, Finished FROM tMessage WHERE 1=1");
		if (order.length() > 0) sql.concat(order);

		sql.concat(string(";"));

		if (connection->executeWithResults(sql, recordset))
		{
			while (recordset->MoveNext())
			{
				records::message temp;
				
				string messageID = recordset->GetString(1L);
				messageID.toChar(temp.messageID, records::message::MAX);

				recordset->GetString(2L).toChar(temp.user, records::message::MAX);
				recordset->GetString(3L).toChar(temp.GUID, records::message::MAX);
				recordset->GetString(4L).toChar(temp.APIKey, records::message::MAX);
				temp.created = (TIMESTAMP_STRUCT)recordset->GetDateTime(5L);
				temp.finished = (TIMESTAMP_STRUCT)recordset->GetDateTime(6L);

				data.push_back(temp);
				identities.push_back(messageID);
			};
		}
		else return false;
	}
	else return false;

	return (data.size() > 0);
}