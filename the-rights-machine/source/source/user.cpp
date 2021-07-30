#include "user.h"
#include "guid.h"
#include "log.h"

void data::user::reset()
{
	userID = 0;
	username = "";
	email = "";
	apikey.clear();
	guid.clear();
	//memset(&apikey, 0, sizeof(GUID));
	//memset(&guid, 0, sizeof(GUID));
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
	//if (!((string)guid::guid(apikey)).icompare(message.apikey)) return false;
	//if (!((string)guid::guid(guid)).icompare(message.user)) return false;

	return true;
}

void data::user::copy(user const &source)
{
	userID = source.userID;
	username = source.username;
	email = source.email;
	apikey = source.apikey;
	guid = source.guid;
	//memcpy(&apikey, &source.apikey, sizeof(GUID));
	//memcpy(&guid, &source.guid, sizeof(GUID));
	active = source.active;
	banned = source.banned;
	verified = source.verified;
}

string data::user::output()
{
	string result("\"user\" : {\"userID\" : ");
	result.concat(string::fromInt(userID));
	result.concat(string(", \"username\" : "));
	result.concat(username);
	result.concat(string(", \"email\" : \""));
	result.concat(email);
	result.concat(string(", \"apikey\" : \"'"));
	result.concat((string)guid::guid(apikey));
	result.concat(string("', \"guid\" : \"'"));
	result.concat((string)guid::guid(guid));
	result.concat(string("', \"active\" : \""));
	result.concat(string::fromBool(active));
	result.concat(string(", \"banned\" : \""));
	result.concat(string::fromBool(banned));
	result.concat(string(", \"verified\" : \""));
	result.concat(string::fromBool(verified));
	result.concat(string("\"}\r\n"));

	return result;
}
