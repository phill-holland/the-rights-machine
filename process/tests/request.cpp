#include "stdafx.h"
#include "request.h"

string tests::data::request::json()
{
	string result = "{\"message\":\r\n{\r\n\"userID\":\"" + userID + "\",\r\n";
	result += "\"APIKey\":\"" + APIKey + "\",\r\n";
	result += "\"GUID\":\"" + GUID + "\"\r\n";
	result += "}\r\n";

	return result;
}