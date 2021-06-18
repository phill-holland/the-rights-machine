#include "guid.h"

/*
guid::guid::guid(string &source)
{
	sscanf_s(source.c_str(), "%08lX-%04hX-%04hX-%02hhX%02hhX-%02hhX%02hhX%02hhX%02hhX%02hhX%02hhX",
			 &value.Data1, &value.Data2, &value.Data3,
			 &value.Data4[0], &value.Data4[1], &value.Data4[2], &value.Data4[3],
			 &value.Data4[4], &value.Data4[5], &value.Data4[6], &value.Data4[7]);

	init = true;
}

void guid::guid::reset()
{
	init = false;

	if (CoCreateGuid(&value) != S_OK) return;

	init = true;
}

string guid::guid::get()
{
	char result[37];
	
	sprintf_s(result,"%08lX-%04hX-%04hX-%02hhX%02hhX-%02hhX%02hhX%02hhX%02hhX%02hhX%02hhX",
			  value.Data1,    value.Data2,    value.Data3,
			  value.Data4[0], value.Data4[1], value.Data4[2], value.Data4[3],
			  value.Data4[4], value.Data4[5], value.Data4[6], value.Data4[7]);
			  
	return string(result);
}
*/