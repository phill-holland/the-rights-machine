#include "element.h"
#include <vector>
#include "../process/string.h"

#if !defined(__COMPONENT)
#define __COMPONENT

class component
{
public:
	string name;

	std::vector<element> elements;

public:
	string json();
};

#endif