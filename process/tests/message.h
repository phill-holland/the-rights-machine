#include "line.h"
#include "item.h"
#include <vector>
#include "../process/string.h"

#if !defined(__MESSAGE)
#define __MESSAGE

class message
{
public:
	string userID;
	string APIKey;

	std::vector<line> queries;
	std::vector<item> items;

public:
	string json();
};

#endif