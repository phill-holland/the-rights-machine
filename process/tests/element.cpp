#include "element.h"

string element::json()
{
	string result = "{\"value\":\"" + value + "\"}\r\n";
	return result;
}