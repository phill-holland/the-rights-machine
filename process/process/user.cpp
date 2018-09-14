#include "user.h"
#include "log.h"

void data::user::reset()
{
	userID = 0;
	username = "";
	email = "";
	apikey = "";
	guid = "";
	active = false;
	banned = true;
	verified = false;
}

void data::user::output()
{
	string result = "\"user\" : {\"userID\" : ";
	result += string::fromInt(userID);
	result += ", \"username\" : ";
	result += username;
	result += ", \"email\" : \"";
	result += email;
	result += ", \"apikey\" : \"";
	result += apikey;
	result += ", \"guid\" : \"";
	result += guid;
	result += ", \"active\" : \"";
	result += string::fromBool(active);
	result += ", \"banned\" : \"";
	result += string::fromBool(banned);
	result += ", \"verified\" : \"";
	result += string::fromBool(verified);
	result += "\"}\r\n";

	Log << result;
}
