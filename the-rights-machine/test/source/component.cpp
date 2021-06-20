#include "component.h"

string tests::data::component::json()
{
	string result = "{\r\n\"name\":\"" + name + "\"";
	if ((long)elements.size() > 0L)
	{
		result += ",\r\n\"elements\":\r\n[\r\n";
		for (long i = 0L; i < (long)elements.size(); ++i)
		{
			result += elements[i].json();
			if (i != (long)elements.size() - 1L) result += ",";
			result += "\r\n";
		}
		result += "]\r\n";
	}
	result += "}\r\n";

	return result;
}