
#include "custom/string.h"

#if !defined(__GUID)
#define __GUID

namespace guid
{
	class guid
	{
		//GUID value;

		bool init;

	public:
		guid() { reset(); }
		guid(guid &unique) {/* value = unique;*/ init = true; }
		guid(string &source);

		void clear() { }

		bool initalised() { return init; }
		void reset();

		string get();
		void generate() { }

	public:
		operator string() { return get(); }
		//operator GUID() { return value; }
	};
};

#endif