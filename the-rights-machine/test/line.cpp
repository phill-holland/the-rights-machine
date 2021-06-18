#include "stdafx.h"
#include "line.h"

string tests::data::line::json()
{
	string result = "{\r\n\"start\":\"" + (string)start + "\",\r\n";
	result += "\"end\":\"" + (string)end + "\",\r\n";
	result += "\"exclusivity\":" + exclusivity + ",\r\n";
	result += "\"type\":" + type + "\r\n";
	if ((long)components.size() > 0L)
	{
		result += ",\r\n\"components\":\r\n[\r\n";
		for (long i = 0L; i < (long)components.size(); ++i)
		{
			result += components[i].json();
			if (i != (long)components.size() - 1L) result += ",";
			result += "\r\n";
		}
		result += "]\r\n";
	}
	result += "}\r\n";

	return result;
}