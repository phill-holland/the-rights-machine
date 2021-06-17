#include "codes.h"

void error::codes::reset(database::settings &settings)
{
	init = false; cleanup();

	this->location = settings.getLocation();

	connection = settings.getConnections()->get();
	if (connection == NULL) return;

	recordset = settings.getRecordSets()->get();
	if (recordset == NULL) return;

	refresh();

	init = true;
}

bool error::codes::get(::error::type::type &destination)
{
	if (data.size() == 0) refresh();

	if (data.size() > 0)
	{
		destination = data.back();
		data.pop_back();

		return true;
	}

	return false;
}

void error::codes::refresh()
{
	mutex lock(token);

	data.clear();

	if (connection->open(location))
	{
		string sql("SELECT Name, Description FROM tCodes WHERE (1=1) ORDER BY CodeID");

		if (connection->executeWithResults(sql, recordset))
		{
			recordset->MoveNext();

			while (recordset->IsInitalised())
			{
				::error::type::type temp;

				
				temp.name = recordset->GetString(1L);
				temp.description = recordset->GetString(2L);

				data.push_back(temp);

				recordset->MoveNext();
			}

			recordset->close();
		}

		connection->close();
	}
}

void error::codes::makeNull()
{
}

void error::codes::cleanup()
{
}