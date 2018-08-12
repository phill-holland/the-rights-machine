#include "element.h"

string data::element::json()
{
	string result = "{\"value\":\"" + value + "\"}\r\n";
	return result;
}