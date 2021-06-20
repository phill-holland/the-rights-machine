#include "request.h"

string tests::data::request::json()
{
	string result = "{\"request\":\r\n{\r\n\"APIKey\":\"" + APIKey + "\",\r\n";
	result += "\"GUID\":\"" + GUID + "\",\r\n";
	result += "\"user\":\"" + user + "\"\r\n";
	result += "}\r\n}\r\n";

	return result;
}