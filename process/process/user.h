#include "message.h"
#include "string.h"
#include "datetime.h"

#if !defined(__USER)
#define __USER

namespace data
{
	class user
	{
	public:
		int userID;
		string username;
		string email;
		GUID apikey;
		GUID guid;
		bool active;
		bool banned;
		bool verified;

	public:
		user() { reset(); }

		void reset();

		bool validate();		
		bool validate(data::message::message &message);

	public:
		void output();
	};
};

#endif