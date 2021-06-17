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
		GUID guid; // rename, user identity key
		bool active;
		bool banned;
		bool verified;

	public:
		user() { reset(); }
		user(user const &source) { reset(); copy(source); }

		void reset();

		bool validate();		
		bool validate(data::message::message &message);

		bool isempty() { return userID == 0; }

		void copy(user const &source);

	public:
		void output();

	public:
		user& operator=(const user& source)
		{
			this->copy((user&)source);
			return *this;
		}
	};
};

#endif