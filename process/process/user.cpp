#include "user.h"
#include "guid.h"
#include "log.h"

void data::user::reset()
{
	userID = 0;
	username = "";
	email = "";
	active = false;
	banned = true;
	verified = false;
}

bool data::user::validate() 
{ 
	return (active && !banned && verified); 
}

bool data::user::validate(::data::message::message &message)
{
	if (!((string)guid::guid(apikey)).icompare((string)guid::guid(message.apikey))) return false;
	if (!((string)guid::guid(guid)).icompare((string)guid::guid(message.guid))) return false;

	return true;
}

void data::user::output()
{
	string result = "\"user\" : {\"userID\" : ";
	result += string::fromInt(userID);
	result += ", \"username\" : ";
	result += username;
	result += ", \"email\" : \"";
	result += email;
	result += ", \"apikey\" : \"'";
	result += (string)guid::guid(apikey);
	result += "', \"guid\" : \"'";
	result += (string)guid::guid(guid);
	result += "', \"active\" : \"";
	result += string::fromBool(active);
	result += ", \"banned\" : \"";
	result += string::fromBool(banned);
	result += ", \"verified\" : \"";
	result += string::fromBool(verified);
	result += "\"}\r\n";

	Log << result;
}
