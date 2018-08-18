#include "stdafx.h"
#include "element.h"

string tests::data::element::json()
{
	string result = "{\"value\":\"" + value + "\"}\r\n";
	return result;
}