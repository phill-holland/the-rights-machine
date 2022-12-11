#include "compute/result.h"

void compute::result::reset()
{
	init = false; cleanup();

	clear();

	init = true;
}

void compute::result::clear()
{
	messageID = 0;
	itemID = 0;
	lineID = 0;
	componentID = 0;

	elements.clear();
}

void compute::result::copy(result const &source)
{
	messageID = source.messageID;
	itemID = source.itemID;
	lineID = source.lineID;
	componentID = source.componentID;

	elements.copy(source.elements);
}

void compute::result::makeNull()
{
}

void compute::result::cleanup()
{
}