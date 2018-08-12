#include "item.h"

string item::json()
{
	string result = "{\r\n\"name\":\"" + name + "\"";
	if ((long)lines.size() > 0L)
	{
		result += ",\r\n\"lines\":\r\n[\r\n";
		for (long i = 0L; i < (long)lines.size(); ++i)
		{
			result += lines[i].json();
			if (i != (long)lines.size() - 1L) result += ",";
			result += "\r\n";
		}
		result += "]\r\n";
	}
	result += "}\r\n";
}