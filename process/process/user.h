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
		string apikey;
		string guid;
		bool active;
		bool banned;
		bool verified;

	public:
		user() { reset(); }

		void reset();

		bool validate() { return (active && !banned&& verified); }
		
	public:
		void output();
	};
};

#endif