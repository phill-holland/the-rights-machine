#include "database.h"

DWORD WINAPI queues::database::incoming::queue::background(thread *bt)
{
	// if inteval ticked, or one of the buffers >INPUT or OUTPUT
	//go for it

	// get

	// insert

	if (counter > interval)
	{
		counter = 0; 
		//refresh();
	}

	++counter;

	Sleep(1000);

	return (DWORD)0;

	return (DWORD)0;
}

void queues::database::incoming::queue::reset(::database::settings &settings, unsigned long interval = INTERVAL)
{
	init = false; cleanup();

	counter = 0UL;

	this->location = settings.getLocation();
	this->interval = interval;

	connection = settings.getConnections()->get();
	if (connection == NULL) return;

	recordset = settings.getRecordSets()->get();
	if (recordset == NULL) return;

	incoming = new custom::fifo<data::message::message, LENGTH>();
	if (incoming == NULL) return;
	if (!incoming->initalised()) return;

	outgoing = new custom::fifo<data::message::message, LENGTH>();
	if (outgoing == NULL) return;
	if (!outgoing->initalised()) return;

	init = true;
}

bool queues::database::incoming::queue::set(data::message::message &source)
{
	if (incoming->entries() >= INPUT)
	{
		// push to database
	}

	return incoming->set(source);
	// shove into incoming queue
	// if queue exceeds X amount
	// flush to database
}

bool queues::database::incoming::queue::get(data::message::message &destination)
{
	if (outgoing->entries() == 0L)
	{
		// also need some sort of timeout, don't want to keep
		// hitting the database

		// pull from database
	}

	return outgoing->get(destination);
	// if nothing in buffer, load 100L items from database into buffer
	// reutrn buffer
}

bool queues::database::incoming::queue::flush()
{
	// flush "put" queue to database

	if (incoming->entries() > 0UL)
	{
		// push to database
	}

	return true;
}

bool queues::database::incoming::queue::write()
{
}

bool queues::database::incoming::queue::read()
{
	mutex lock(token);

	if (connection->open(location))
	{
		string sql("SELECT userID, username, email, apikey, GUID, banned, verified, active FROM tUser WHERE (1=1)");
		
		if (connection->executeWithResults(sql, recordset))
		{
			recordset->MoveNext();

			while (recordset->IsInitalised())
			{
				/*
				data::user temp;

				temp.userID = recordset->GetLong(1L);
				temp.username = recordset->GetString(2L);
				temp.email = recordset->GetString(3L);
				temp.apikey = recordset->GetString(4L);
				temp.guid = recordset->GetString(5L);
				temp.banned = recordset->GetBool(6L);
				temp.verified = recordset->GetBool(7L);
				temp.active = recordset->GetBool(8L);
				*/
				recordset->MoveNext();
			}

			recordset->close();

			connection->executeNoResults("DELETE FROM tUserChange");
		}

		connection->close();
	}
}

void queues::database::incoming::queue::makeNull()
{
	incoming = NULL;
	outgoing = NULL;
}

void queues::database::incoming::queue::cleanup()
{
	if (outgoing != NULL) delete outgoing;
	if (incoming != NULL) delete incoming;
}