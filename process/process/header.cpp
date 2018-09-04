#include "header.h"

void compute::header::clear()
{
	messageID = 0;
	itemID = 0;
	lineID = 0;
	componentID = 0;
}

bool compute::header::isempty()
{
	return ((messageID == 0) && (itemID == 0) && (lineID == 0) && (componentID == 0));
}

string compute::header::serialize()
{
	string result = string("{\"header:\":{\"m\":");
	result += string(messageID);
	result += string(",\"i\":");
	result += string(itemID);
	result += string(",\"l\":");
	result += string(lineID);
	result += string(",\"c\":");
	result += string(componentID);
	result += string("} }");

	return result;
}

void compute::header::copy(header const &source)
{
	messageID = source.messageID;
	itemID = source.itemID;
	lineID = source.lineID;
	componentID = source.componentID;
}