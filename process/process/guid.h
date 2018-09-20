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
		guid(GUID &unique) { value = unique; init = true; }
		guid(string &source)
		{
			sscanf_s(source.c_str(), "{%8x-%4hx-%4hx-%2hhx%2hhx-%2hhx%2hhx%2hhx%2hhx%2hhx%2hhx}",
				    &value.Data1, &value.Data2, &value.Data3,
				    &value.Data4[0], &value.Data4[1], &value.Data4[2], &value.Data4[3],
				    &value.Data4[4], &value.Data4[5], &value.Data4[6], &value.Data4[7], source.size());

			init = true;
		}

		bool initalised() { return init; }
		void reset();

		string get();

	public:
		operator string() { return get(); }
		operator GUID() { return value; }
	};
};

#endif