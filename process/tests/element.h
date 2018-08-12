#include "../process/string.h"

#if !defined(__ELEMENT)
#define __ELEMENT

class element
{
public:
	string value;

public:
	string json();
};

#endif