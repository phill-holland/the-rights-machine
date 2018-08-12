#include "line.h"
#include <vector>
#include "../process/string.h"

#if !defined(__ITEM)
#define __ITEM

class item
{
public:
	string name;

	std::vector<line> lines;

public:
	string json();
};

#endif