#include "watchdog.h"
#include "guid.h"
#include "datetime.h"

DWORD WINAPI watchdog::watchdog::background(thread *bt)
{
	if (counter > interval)
	{
		counter = 0; refresh();
	}

	++counter;

	Sleep(1000);

	return (DWORD)0;
}

void watchdog::watchdog::reset(database::settings &settings, unsigned long interval)
{
	init = false; cleanup();

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

void watchdog::watchdog::refresh()
{
	if (connection->open(location))
	{
		GUID unique = (GUID)guid::guid();

		string sql("INSERT INTO tWatchdog (watchdogID, updated) VALUES(?,?)");

		if (connection->Prepare(sql, recordset))
		{
			recordset->BindGUID(1L, unique);
			recordset->BindTimeStamp(2L, (TIMESTAMP_STRUCT)global::datetime::now());

			if (connection->executeNoResults(sql))
			{
				sql = string("DELETE FROM tWatchdog WHERE watchdogID !=?");

				if (connection->Prepare(sql, recordset))
				{
					recordset->BindGUID(1L, unique);
					connection->executeNoResults(sql);
				}
			}
		}

		recordset->close();
		connection->close();
	}
}

void watchdog::watchdog::makeNull()
{
}

void watchdog::watchdog::cleanup()
{
}