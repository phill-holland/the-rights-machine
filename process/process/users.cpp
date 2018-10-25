#include "users.h"
#include "guid.h"

using namespace comparison;

DWORD WINAPI data::users::background(thread *bt)
{
	if (counter > interval)
	{
		counter = 0; refresh();
	}

	++counter;

	Sleep(1000);

	return (DWORD)0;
}

void data::users::reset(database::settings &settings, unsigned long interval)
{
	init = false; cleanup();

	first = true;

	counter = 0UL;

	this->location = settings.getLocation();
	this->interval = interval;

	connection = settings.getConnections()->get();
	if (connection == NULL) return;

	recordset = settings.getRecordSets()->get();
	if (recordset == NULL) return;

	refresh();

	init = true;
}

data::user data::users::get(string user)
{
	mutex lock(token);

	std::unordered_map<string, data::user, hasher, equality>::iterator i = map.find(user);
	if (i != map.end())
	{
		data::user temp = ((data::user)i->second);
		if (temp.validate()) return temp;
	}

	return data::user();
}

void data::users::refresh()
{
	mutex lock(token);

	if (connection->open(location))
	{
		string sql("SELECT userID, username, email, apikey, GUID, banned, verified, active FROM tUser WHERE (1=1)");
		if (!first) sql.concat(" AND userID IN (SELECT userID FROM tUserChange)");

		if (connection->executeWithResults(sql, recordset))
		{
			first = false;
		
			recordset->MoveNext();

			while (recordset->IsInitalised())
			{
				data::user temp;

				temp.userID = recordset->GetLong(1L);
				temp.username = recordset->GetString(2L);
				temp.email = recordset->GetString(3L);
				temp.apikey = recordset->GetGUID(4L);
				temp.guid = recordset->GetGUID(5L);
				temp.banned = recordset->GetBool(6L);
				temp.verified = recordset->GetBool(7L);
				temp.active = recordset->GetBool(8L);

				map[(string)guid::guid(temp.guid)] = temp;

				recordset->MoveNext();
			}

			recordset->close();
			
			connection->executeNoResults("DELETE FROM tUserChange");
		}

		connection->close();
	}
}

void data::users::makeNull()
{
}

void data::users::cleanup()
{
}