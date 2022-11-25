
#include "core/string/string.h"
#include <uuid/uuid.h>

#if !defined(__GUID)
#define __GUID

namespace guid
{
	//https://stackoverflow.com/questions/2174768/generating-random-uuids-in-linux

	class guid
	{
		uuid_t value;
		//GUID value;	
	/*	unsigned int Data1;	
		unsigned short Data2;	
		unsigned short Data3;
		unsigned char Data4[8];
*/
		bool init;

	public:
		guid() { reset(); }
		guid(const guid &unique) { /*value = unique.value;*/ init = true; }
		guid(const string &source);

		void clear() { reset(); }

		bool initalised() { return init; }
		void reset();

		string get();
		void generate() { reset(); }

	public:
		operator string() { return get(); }
		//operator GUID() { return value; }
	};
};

#endif