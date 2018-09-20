#include "request.h"
#include "log.h"

void data::request::request::clear()
{
	requestID = 0;
	user = string("");
	guid = string("");
}

data::json::request::json *data::request::request::find(string FQDN)
{
	if (FQDN.icompare(this->FQDN())) return this;
	
	return NULL;
}

void data::request::request::copy(request const &source)
{
	requestID = source.requestID;
	user = source.user;
	guid = source.guid;
}

void data::request::request::output()
{
	string result = "\"request\" : {\"requestID\" : ";
	result += string::fromInt(requestID);
	result += ", \"user\" : ";
	result += user;
	result += ", \"GUID\" : \"'";
	result += guid;
	result += "'\"}\r\n";

	Log << result;
}

bool data::request::request::add(custom::pair &source)
{
	if (string("GUID").icompare(source.name))
	{
		guid = source.value;
		return true;
	}

	if (string("user").icompare(source.name))
	{
		user = source.value;
		return true;
	}

	return false;
}