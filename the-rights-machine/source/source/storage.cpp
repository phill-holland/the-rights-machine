#include "storage.h"

bool database::storage::request::open(database::settings &settings)
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

bool database::storage::request::close()
{
	recordset->close();
	if (!connection->close()) return false;

	return true;
}

bool database::storage::request::read(data::request::request &destination)
{
	if (data.size() == 0)
	{
		if (!load()) return false;
	}

	if (data.size() > 0)
	{
		records::request temp = data.back();
		data.pop_back();

		destination.user = (string)guid::guid(temp.user);
		destination.guid = (string)guid::guid(temp.guid);
		destination.created = (datetime)temp.created;

		return true;
	}

	return false;
}

bool database::storage::request::write(data::request::request &source)
{
	bool prepared = true;

	if (!recordset->IsInitalised())
	{
		string sql = "INSERT INTO tRequest (requestID, [user], [GUID], created)";
		sql.concat(" VALUES(?,?,?,?);");

		if (!connection->Prepare(sql, recordset)) prepared = false;
	}


	if ((recordset->IsInitalised()) && (prepared))
	{
		bound.set(source);

		guid::guid unique;// = this->generate();
		unique.generate();
		bound.requestID = unique;

		bound.bind(recordset);

		if (!recordset->Execute()) return false;

		return true;
	}

	return false;
}

bool database::storage::request::load()
{
	clear();

	if (!recordset->IsInitalised())
	{
		guid::guid tagged;// = this->generate();
		tagged.generate();
		if (!tag(tagged)) return false;

		string order = "";

		string sql("SELECT");
		if (max > 0L)
		{
			sql.concat(" TOP ");
			sql.concat(string::fromLong(max));

			order = " ORDER BY created DESC";
		}

		sql.concat(" requestID, [User], [GUID], Tag, Created FROM tRequest WHERE Tag=?");
		if (order.length() > 0) sql.concat(order);
		sql.concat(string(";"));

		recordset->BindGUID(1L, tagged);

		if (connection->executeWithResults(sql, recordset))
		{
			while (recordset->MoveNext())
			{
				records::request temp;

				temp.requestID = recordset->GetGUID(1L);
				temp.user = recordset->GetGUID(2L);
				temp.guid = recordset->GetGUID(3L);
				temp.tag = recordset->GetGUID(4L);
				temp.created = recordset->GetTimeStamp(5L);

				//data.push_back(temp);
				identities.push_back((string)guid::guid(temp.requestID));
			};
		}
		else return false;
	}
	else return false;

	return (data.size() > 0);
}

bool database::storage::request::tag(guid::guid &tagged)
{
	string sql = "UPDATE tRequest SET Tag=? WHERE tRequest.RequestID IN (SELECT TOP ";
	sql.concat(string::fromLong(TOP));
	sql.concat(" rqst.requestID FROM tRequest AS rqst WHERE rqst.Tag IS NULL ORDER BY rqst.Created);");

	if (!connection->Prepare(sql, recordset)) return false;

	recordset->BindGUID(1L, tagged);

	return connection->executeNoResults(sql);
}

bool database::storage::request::erase(guid::guid &tagged)
{
	string sql = "DELETE FROM tRequest WHERE Tag=?;";

	if (!connection->Prepare(sql, recordset)) return false;

	recordset->BindGUID(1L, tagged);

	return connection->executeNoResults(sql);
}

// ***

bool database::storage::response::open(database::settings &settings)
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

bool database::storage::response::close()
{
	recordset->close();
	if (!connection->close()) return false;

	return true;
}

bool database::storage::response::read(data::response::response &destination)
{
	if (data.size() == 0)
	{
		if (!load()) return false;
	}

	if (data.size() > 0)
	{
		records::response temp = data.back();
		data.pop_back();

		destination.guid = (string)guid::guid(temp.guid);
		destination.user = (string)guid::guid(temp.user);
		destination.status = (data::response::response::STATUS)temp.status;
		destination.created = (datetime)temp.created;
		destination.available = temp.available;

		return true;
	}

	return false;
}

bool database::storage::response::write(data::response::response &source)
{
	bool prepared = true;

	if (!recordset->IsInitalised())
	{
		string sql = "INSERT INTO tResponse (responseID, [guid], [user], status, created, available)";
		sql.concat(" VALUES(?,?,?,?,?,?);");

		if (!connection->Prepare(sql, recordset)) prepared = false;
	}


	if ((recordset->IsInitalised()) && (prepared))
	{
		bound.set(source);

		guid::guid unique;
		unique.generate();
		bound.responseID = unique;

		bound.bind(recordset);

		if (!recordset->Execute()) return false;

		return true;
	}

	return false;
}

bool database::storage::response::load()
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

		sql.concat(" responseID, [Guid], [User], Status, Created, Available FROM tResponse WHERE (1=1)");
		if (order.length() > 0) sql.concat(order);
		sql.concat(string(";"));

		if (connection->executeWithResults(sql, recordset))
		{
			while (recordset->MoveNext())
			{
				records::response temp;

				temp.responseID = recordset->GetGUID(1L);
				temp.guid = recordset->GetGUID(2L);
				temp.user = recordset->GetGUID(3L);
				temp.status = recordset->GetLong(4L);
				temp.created = recordset->GetTimeStamp(5L);
				temp.available = recordset->GetBool(6L);

				//data.push_back(temp);
				identities.push_back((string)guid::guid(temp.responseID));
			};
		}
		else return false;
	}
	else return false;

	return (data.size() > 0);
}
// ****

bool database::storage::common::line::element::open(database::settings &settings)
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

bool database::storage::common::line::element::close()
{
	recordset->close();
	if (!connection->close()) return false;

	return true;
}

bool database::storage::common::line::element::read(data::element::element &destination)
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

bool database::storage::common::line::element::write(data::element::element &source)
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

		guid::guid unique;
		unique.generate();
		bound.componentID = componentID;
		bound.elementID = unique;

		bound.bind(recordset);

		if (!recordset->Execute()) return false;

		return true;
	}

	return false;
}

bool database::storage::common::line::element::load()
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

bool database::storage::common::line::component::open(database::settings &settings)
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

	if (!t_element.open(settings))
	{
		recordset->close();
		connection->close();

		return false;
	}

	return true;
}

bool database::storage::common::line::component::close()
{
	int index = 0;

	recordset->close();

	if (!t_element.close()) ++index;
	if (!connection->close()) ++index;

	return index == 0;
}

bool database::storage::common::line::component::read(data::component::line::component &destination)
{
	string key = (string)guid::guid(lineID);

	std::unordered_map<string, std::vector<database::records::component::line::component>, hasher, equality>::iterator i = data.find(key);
	if (i == data.end())
	{
		if (!load()) return false;

		t_element.identities = identities;
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
			t_element.componentID = temp.componentID;
			t_element.parent = parent;
			if (!parent->load(&t_element)) return false;
		}

		return true;
	}

	return false;
}

bool database::storage::common::line::component::write(data::component::line::component &source)
{
	bool prepared = true;

	if (!recordset->IsInitalised())
	{
		string sql = "INSERT INTO tComponent (componentID, [ID], [Type], name)";
		sql.concat(" VALUES(?,?,?,?);");

		if (!connection->Prepare(sql, recordset)) prepared = false;
	}

	if ((recordset->IsInitalised()) && (prepared))
	{
		bound.set(source);

		guid::guid unique;// = this->generate();
		unique.generate();
		bound.lineID = lineID;
		bound.componentID = unique;

		bound.bind(recordset);

		if (!recordset->Execute()) return false;

		if (parent != NULL)
		{
			t_element.componentID = unique;
			t_element.parent = parent;
			if (!parent->save(&t_element)) return false;
		}

		return true;
	}

	return false;
}

bool database::storage::common::line::component::load()
{
	clear();

	if (!recordset->IsInitalised())
	{
		string sql("SELECT componentID, [ID], name FROM tComponent ");
		sql.concat("LEFT JOIN tLine ON tComponent.lineID = tLine.lineID ");
		sql.concat("LEFT jOIN tItem ON tLine.itemID = tItem.itemID ");
		sql.concat("WHERE tComponent.Type=1");

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

bool database::storage::common::query::element::open(database::settings &settings)
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

bool database::storage::common::query::element::close()
{
	recordset->close();
	if (!connection->close()) return false;

	return true;
}

bool database::storage::common::query::element::read(data::element::element &destination)
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

bool database::storage::common::query::element::write(data::element::element &source)
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

		guid::guid unique;// = this->generate();
		unique.generate();
		bound.componentID = componentID;
		bound.elementID = unique;

		bound.bind(recordset);

		if (!recordset->Execute()) return false;

		return true;
	}

	return false;
}

bool database::storage::common::query::element::load()
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

bool database::storage::common::query::component::open(database::settings &settings)
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

	if (!t_element.open(settings))
	{
		recordset->close();
		connection->close();

		return false;
	}

	return true;
}

bool database::storage::common::query::component::close()
{
	int index = 0;

	recordset->close();

	if (!t_element.close()) ++index;
	if (!connection->close()) ++index;

	return index == 0;
}

bool database::storage::common::query::component::read(data::component::query::component &destination)
{
	string key = (string)guid::guid(queryID);

	std::unordered_map<string, std::vector<database::records::component::query::component>, hasher, equality>::iterator i = data.find(key);
	if (i == data.end())
	{
		if (!load()) return false;

		t_element.identities = identities;
	}

	std::unordered_map<string, std::vector<database::records::component::query::component>, hasher, equality>::iterator j = data.find(key);
	if (j == data.end()) return false;

	std::vector<database::records::component::query::component> source = data[key];

	if (source.size() > 0)
	{
		database::records::component::query::component temp = data[key].back();
		data[key].pop_back();

		destination.name = (string)temp.name;

		if (parent != NULL)
		{
			t_element.componentID = temp.componentID;
			t_element.parent = parent;
			if (!parent->load(&t_element)) return false;
		}

		return true;
	}

	return false;
}

bool database::storage::common::query::component::write(data::component::query::component &source)
{
	bool prepared = true;

	if (!recordset->IsInitalised())
	{
		string sql = "INSERT INTO tComponent (componentID, [ID], [Type], name)";
		sql.concat(" VALUES(?,?,?,?);");

		if (!connection->Prepare(sql, recordset)) prepared = false;
	}

	if ((recordset->IsInitalised()) && (prepared))
	{
		guid::guid unique;// = this->generate();
		unique.generate();
		bound.queryID = queryID;
		bound.componentID = unique;

		bound.bind(recordset);

		if (!recordset->Execute()) return false;

		if (parent != NULL)
		{
			t_element.componentID = unique;
			t_element.parent = parent;
			if (!parent->save(&t_element)) return false;
		}

		return true;
	}

	return false;
}

bool database::storage::common::query::component::load()
{
	clear();

	if (!recordset->IsInitalised())
	{
		string sql("SELECT componentID, [ID], name FROM tComponent ");
		sql.concat("LEFT JOIN tQuery ON tComponent.ID = tQuery.ID ");
		sql.concat("WHERE tComponent.Type=2");

		if (identities.size() > 0UL)
		{
			sql.concat(" AND (");
			for (unsigned long i = 0UL; i < identities.size(); ++i)
			{
				string temp = identities[i];
				if (i > 0UL) sql.concat(" OR ");
				sql.concat("tQuery.messageID='");
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
				database::records::component::query::component temp;

				temp.componentID = recordset->GetGUID(1L);
				temp.queryID = recordset->GetGUID(2L);
				recordset->GetString(3L).toChar(temp.name, database::records::component::query::component::MAX);

				data[(string)guid::guid(queryID)].push_back(temp);
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

		guid::guid unique;// = this->generate();
		unique.generate();
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

bool database::storage::query::open(database::settings &settings)
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

bool database::storage::query::close()
{
	int index = 0;

	recordset->close();

	if (!component.close()) ++index;
	if (!connection->close()) ++index;

	return index == 0;
}

bool database::storage::query::read(data::query::query &destination)
{
	string key = (string)guid::guid(messageID);

	std::unordered_map<string, std::vector<records::query>, hasher, equality>::iterator i = data.find(key);
	if (i == data.end())
	{
		if (!load()) return false;

		component.identities = identities;
	}

	std::unordered_map<string, std::vector<records::query>, hasher, equality>::iterator j = data.find(key);
	if (j == data.end()) return false;

	std::vector<records::query> source = data[key];
	if (source.size() > 0)
	{
		records::query temp = data[key].back();
		data[key].pop_back();

		if (parent != NULL)
		{
			component.queryID = temp.queryID;
			component.parent = &destination;
			if (!destination.load(&component)) return false;
		}

		return true;
	}

	return false;
}

bool database::storage::query::write(data::query::query &source)
{
	bool prepared = true;

	if (!recordset->IsInitalised())
	{
		string sql = "INSERT INTO tQuery (queryID, messageID)";
		sql.concat(" VALUES(?,?);");

		if (!connection->Prepare(sql, recordset)) prepared = false;
	}

	if ((recordset->IsInitalised()) && (prepared))
	{
		bound.set(source);

		guid::guid unique;// = this->generate();
		unique.generate();
		bound.messageID = messageID;
		bound.queryID = unique;

		bound.bind(recordset);

		if (!recordset->Execute()) return false;

		if (parent != NULL)
		{
			component.queryID = unique;
			component.parent = &source;
			if (!source.save(&component)) return false;
		}

		return true;
	}

	return false;
}

bool database::storage::query::load()
{
	clear();

	if (!recordset->IsInitalised())
	{
		string sql("SELECT queryID, messageID FROM tQuery WHERE 1=1");

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
				records::query temp;

				temp.queryID = recordset->GetGUID(1L);
				temp.messageID = recordset->GetGUID(2L);

				data[(string)guid::guid(temp.messageID)].push_back(temp);
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

	if (!t_line.open(settings))
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

	if (!t_line.close()) ++index;
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

		t_line.identities = identities;
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
			t_line.itemID = temp.itemID;
			t_line.parent = parent;
			if (!parent->load(&t_line)) return false;
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

		guid::guid unique;// = this->generate();
		unique.generate();
		bound.messageID = messageID;
		bound.itemID = unique;

		bound.bind(recordset);

		if (!recordset->Execute()) return false;

		if (parent != NULL)
		{
			t_line.itemID = unique;
			t_line.parent = parent;
			if (!parent->save(&t_line)) return false;
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

	if (!t_item.open(settings))
	{
		recordset->close();
		connection->close();

		return false;
	}

	if (!t_query.open(settings))
	{
		t_item.close();
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

	if (!t_query.close()) ++index;
	if (!t_item.close()) ++index;
	if (!connection->close()) ++index;

	return index == 0;
}

bool database::storage::message::read(data::message::message &destination)
{
	if (data.size() == 0)
	{		
		if (!load()) return false;

		t_item.identities = identities;		
	}

	if (data.size() > 0)
	{		
		records::message temp = data.back();
		data.pop_back();

		destination.user = (string)guid::guid(temp.user);
		destination.guid = (string)guid::guid(temp.guid);
		destination.apikey = (string)guid::guid(temp.apikey);
		destination.created = (datetime)temp.created;
		
		t_item.messageID = temp.messageID;		
		t_item.parent = &destination;
		if (!destination.load(&t_item)) return false;

		t_query.messageID = temp.messageID;
		t_item.parent = &destination;
		if (!destination.load(&t_query)) return false;

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

		guid::guid unique;// = this->generate();
		unique.generate();
		bound.messageID = unique;

		bound.bind(recordset);

		if (!recordset->Execute()) return false;

		t_item.messageID = unique;
		t_item.parent = &source;
		if (!source.save(&t_item)) return false;

		t_query.messageID = unique;
		t_item.parent = &source;
		if (!source.save(&t_query)) return false;

		return true;
	}

	return false;
}

bool database::storage::message::load()
{	
	clear();

	if (!recordset->IsInitalised())
	{
		guid::guid tagged;// = this->generate();		
		tagged.generate();
		if (!tag(tagged)) return false;

		string order = "";

		string sql("SELECT");
		if (max > 0L)
		{
			sql.concat(" TOP ");
			sql.concat(string::fromLong(max));

			order = " ORDER BY created DESC";
		}

		sql.concat(" messageID, [User], [GUID], APIKey, Tag, Created FROM tMessage WHERE Tag=?");
		if (order.length() > 0) sql.concat(order);
		sql.concat(string(";"));

		recordset->BindGUID(1L, tagged);

		if (connection->executeWithResults(sql, recordset))
		{
			while (recordset->MoveNext())
			{
				records::message temp;

				temp.messageID = recordset->GetGUID(1L);
				temp.user = recordset->GetGUID(2L);
				temp.guid = recordset->GetGUID(3L);
				temp.apikey = recordset->GetGUID(4L);
				temp.tag = recordset->GetGUID(5L);
				temp.created = recordset->GetTimeStamp(6L);

				//data.push_back(temp);
				identities.push_back((string)guid::guid(temp.messageID));
			};
		}
		else return false;
	}
	else return false;

	return (data.size() > 0);
}

bool database::storage::message::tag(guid::guid &tagged)
{	
	string sql = "UPDATE tMessage SET Tag=? WHERE tMessage.MessageID IN (SELECT TOP ";
	sql.concat(string::fromLong(TOP));
	sql.concat(" msg.MessageID FROM tMessage AS msg WHERE msg.Tag IS NULL ORDER BY msg.Created);");

	if (!connection->Prepare(sql, recordset)) return false;

	recordset->BindGUID(1L, tagged);

	return connection->executeNoResults(sql);
}

bool database::storage::message::erase(guid::guid &tagged)
{
	string sql = "EXEC dbo.pEraseMessagesWithTag ?;";

	if (!connection->Prepare(sql, recordset)) return false;

	recordset->BindGUID(1L, tagged);

	return connection->executeNoResults(sql);
}