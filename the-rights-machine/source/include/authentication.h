#include "custom/string.h"

#if !defined(__AUTHENTICATION)
#define __AUTHENTICATION

namespace web
{
	class authentication
	{
	public:
		bool required;
		string username, password;

	public:
		authentication() { reset(); }

		void reset()
		{
			required = false;
			username.reset(); password.reset();
		}
	};
};

#endif