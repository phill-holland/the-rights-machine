#include "stdafx.h"
#include "message.h"

string tests::data::message::json()
{
	string result = "{\r\n\"userID\":\"" + userID + "\",\r\n";
	result += "\"APIKey\":\"" + APIKey + "\"";
	if ((long)queries.size() > 0L)
	{
		result += ",\r\n\"queries\":\r\n[\r\n";
		for (long i = 0L; i < (long)queries.size(); ++i)
		{
			result += queries[i].json();
			if (i != (long)queries.size() - 1L) result += ",";
			result += "\r\n";
		}
		result += "]\r\n";
	}

	if ((long)items.size() > 0L)
	{
		result += ",\r\n\"items\":\r\n[\r\n";
		for (long i = 0L; i < (long)items.size(); ++i)
		{
			result += items[i].json();
			if (i != (long)items.size() - 1L) result += ",";
			result += "\r\n";
		}
		result += "]\r\n";
	}
	result += "}\r\n";

	return result;
}