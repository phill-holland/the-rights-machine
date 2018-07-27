#include <windows.h>
#include "string.h"

#if !defined(__GUID)
#define __GUID

namespace guid
{
	class guid
	{
		GUID value;

		bool init;

	public:
		guid() { reset(); }

		bool initalised() { return init; }
		void reset();

		string get();

	public:
		operator string() { return get(); }
	};
};

#endif