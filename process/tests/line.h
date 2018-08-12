#include "component.h"
#include <vector>
#include "../process/string.h"
#include "../process/datetime.h"

#if !defined(__LINE)
#define __LINE

class line
{
public:
	global::datetime start, end;
	string exclusivity;
	string type;

	std::vector<component> components;

public:
	string json();
};

#endif