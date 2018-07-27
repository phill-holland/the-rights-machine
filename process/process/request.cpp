#include "request.h"
#include "log.h"

void data::request::request::clear()
{
	requestID = 0;
	userID = 0;
	GUID = "";
}

void data::request::request::copy(request const &source)
{
	requestID = source.requestID;
	userID = source.userID;
	GUID = source.GUID;
}

void data::request::request::output()
{
	string result = "\"request\" : {\"requestID\" : ";
	result += string::fromInt(requestID);
	result += ", \"userID\" : ";
	result += string::fromInt(userID);
	result += ", \"GUID\" : \"";
	result += GUID;
	result += "\"}\r\n";

	Log << result;
}

bool data::request::request::add(custom::pair &source)
{
	if (string("GUID").icompare(source.name))
	{
		GUID = source.value;
		return true;
	}

	if (string("userID").icompare(source.name))
	{
		userID = source.value.toInteger();
		return true;
	}

	return false;
}