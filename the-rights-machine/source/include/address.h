#include "custom/string.h"

#if !defined(__ADDRESS)
#define __ADDRESS

namespace web
{
	class address
	{
	public:
		string server;
		string path;

		bool secure;

		long port;

	public:
		address() { reset(); }
		address(string &url) { reset(); get(url); }

		void reset();
		bool get(string &url);
	};
};

#endif