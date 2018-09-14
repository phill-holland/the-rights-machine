#include "users.h"

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

void data::users::reset(unsigned long interval, database::settings &settings)
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
				temp.apikey = recordset->GetString(4L);
				temp.guid = recordset->GetString(5L);
				temp.banned = recordset->GetBool(6L);
				temp.verified = recordset->GetBool(7L);
				temp.active = recordset->GetBool(8L);

				map[temp.userID] = temp;

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