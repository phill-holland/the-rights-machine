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
	std::unordered_map<long, std::vector<database::records::element>>::iterator i = data.find(componentID);
	if (i == data.end()) if (!load()) return false;

	std::unordered_map<long, std::vector<database::records::element>>::iterator i = data.find(componentID);
	if (i == data.end()) return false;

	std::vector<database::records::element> source = data[componentID];

	if (source.size() > 0)
	{
		database::records::element temp = data[componentID].back();
		data[componentID].pop_back();

		destination.elementID = temp.elementID;
		destination.componentID = temp.componentID;
		destination.value = string(temp.value);
	}
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
				long temp = identities[i];
				if (i > 0UL) sql.concat(" OR ");
				sql.concat("tItem.messageID=");
				sql.concat(string::fromLong(temp));
			}
			sql.concat(")");
		}

		sql.concat(string(";"));

		if (connection->executeWithResults(sql, recordset))
		{
			while (recordset->MoveNext())
			{
				database::records::element temp;

				temp.elementID = recordset->GetLong(1L);
				temp.componentID = recordset->GetLong(2L);
				recordset->GetString(3L).toChar(temp.value, database::records::element::MAX);

				data[temp.componentID].push_back(temp);
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
	std::unordered_map<long, std::vector<database::records::component::line::component>>::iterator i = data.find(lineID);
	if (i == data.end())
	{
		if (!load()) return false;

		element.identities = identities;
	}

	std::unordered_map<long, std::vector<database::records::component::line::component>>::iterator i = data.find(lineID);
	if (i == data.end()) return false;

	std::vector<database::records::component::line::component> source = data[lineID];

	if (source.size() > 0)
	{
		database::records::component::line::component temp = data[lineID].back();
		data[lineID].pop_back();

		destination.componentID = temp.componentID;
		destination.lineID = temp.lineID;
		destination.name = (string)temp.name;

		if (parent != NULL)
		{
			element.componentID = temp.componentID;
			element.parent = parent;
			if (!parent->load(&element)) return false;
		}
	}
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
				long temp = identities[i];
				if (i > 0UL) sql.concat(" OR ");
				sql.concat("tItem.messageID=");
				sql.concat(string::fromLong(temp));
			}
			sql.concat(")");
		}

		sql.concat(string(";"));

		if (connection->executeWithResults(sql, recordset))
		{
			while (recordset->MoveNext())
			{
				database::records::component::line::component temp;

				temp.componentID = recordset->GetLong(1L);
				temp.lineID = recordset->GetLong(2L);
				recordset->GetString(3L).toChar(temp.name, database::records::component::line::component::MAX);

				data[temp.lineID].push_back(temp);
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
	std::unordered_map<long, std::vector<records::line>>::iterator i = data.find(itemID);
	if (i == data.end())
	{
		if (!load()) return false;

		component.identities = identities;
	}

	std::unordered_map<long, std::vector<records::line>>::iterator i = data.find(itemID);
	if (i == data.end()) return false;

	std::vector<records::line> source = data[itemID];

	if (source.size() > 0)
	{
		records::line temp = data[itemID].back();
		data[itemID].pop_back();

		destination.lineID = temp.lineID;
		destination.itemID = temp.itemID;
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
	}
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
				long temp = identities[i];
				if (i > 0UL) sql.concat(" OR ");
				sql.concat("tItem.messageID=");
				sql.concat(string::fromLong(temp));
			}
			sql.concat(")");
		}

		sql.concat(string(";"));

		if (connection->executeWithResults(sql, recordset))
		{
			while (recordset->MoveNext())
			{
				records::line temp;

				temp.lineID = recordset->GetLong(1L);
				temp.itemID = recordset->GetLong(2L);
				temp.start = recordset->GetDateTime(3L);
				temp.end = recordset->GetDateTime(4L);
				temp.exclusivityID = recordset->GetLong(5L);
				temp.typeID = recordset->GetLong(6L);

				data[temp.lineID].push_back(temp);
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
	std::unordered_map<long, std::vector<records::item>>::iterator i = data.find(messageID);
	if (i == data.end())
	{
		if (!load()) return false;

		line.identities = identities;
	}

	std::unordered_map<long, std::vector<records::item>>::iterator i = data.find(messageID);
	if (i == data.end()) return false;

	std::vector<records::item> source = data[messageID];
	if (source.size() > 0)
	{
		records::item temp = data[messageID].back();
		data[messageID].pop_back();

		destination.itemID = temp.itemID;
		destination.messageID = temp.messageID;
		destination.name = (string)temp.name;

		if (parent != NULL)
		{
			line.itemID = temp.itemID;
			line.parent = parent;
			if (!parent->load(&line)) return false;
		}
	}
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
				long temp = identities[i];
				if (i > 0UL) sql.concat(" OR ");
				sql.concat("messageID=");
				sql.concat(string::fromLong(temp));
			}
			sql.concat(")");
		}

		sql.concat(string(";"));

		if (connection->executeWithResults(sql, recordset))
		{
			while (recordset->MoveNext())
			{
				records::item temp;

				temp.itemID = recordset->GetLong(1L);
				temp.messageID = recordset->GetLong(2L);
				recordset->GetString(3L).toChar(temp.name, records::message::MAX);

				data[temp.messageID].push_back(temp);
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

		destination.messageID = temp.messageID;
		destination.user = (string)temp.user;
		destination.GUID = (string)temp.GUID;
		destination.APIkey = (string)temp.APIKey;
		destination.created = (datetime)temp.created;
		destination.finished = (datetime)temp.finished;
		
		item.messageID = temp.messageID;		
		item.parent = &destination;
		destination.load(&item);
	}
}

bool database::storage::message::write(data::message::message &source)
{
	//identifier();

	if (!recordset->IsInitalised())
	{
		string sql("INSERT INTO tSchema (mindID, type, collided, collided_lifetime, collided_data, collided_data_or_lifetime,");
		sql.concat("collided_join, collided_join_with_join, collided_join_one_side, average_expired_age, average_collision_age,");
		sql.concat("collision_point, collision_point_data, collision_point_lifetime,");
		sql.concat("score, sentences, mother, father, identifier) VALUES(?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?);");

		connection->Prepare(sql, recordset);
	}

	if (recordset->IsInitalised())
	{
		bound.set(source);
		bound.identify(random);

		//bound.mindID = mindID;
		//bound.type = (long)type;

		bound.bind(recordset);

		if (!recordset->Execute()) return false;

		//long l = last();

		/*
		collision.schemaID = l;
		collision.identifier(random);
		if (!source.save(&collision)) return false;

		lifetime.schemaID = l;
		lifetime.identifier(random);
		if (!source.save(&lifetime)) return false;

		inserter.schemaID = l;
		inserter.identifier(random);
		if (!source.save(&inserter)) return false;

		sentencer.schemaID = l;
		sentencer.identifier(random);
		if (!source.save(&sentencer)) return false;
		*/
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
				
				temp.messageID = recordset->GetLong(1L);
				recordset->GetString(2L).toChar(temp.user, records::message::MAX);
				recordset->GetString(3L).toChar(temp.GUID, records::message::MAX);
				recordset->GetString(4L).toChar(temp.APIKey, records::message::MAX);
				temp.created = (TIMESTAMP_STRUCT)recordset->GetDateTime(5L);
				temp.finished = (TIMESTAMP_STRUCT)recordset->GetDateTime(6L);

				data.push_back(temp);
				identities.push_back(temp.messageID);
			};
		}
		else return false;
	}
	else return false;

	return (data.size() > 0);
}