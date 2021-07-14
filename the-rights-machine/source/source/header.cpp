#include "header.h"

void compute::header::clear()
{
	messageID = -1;
	itemID = -1;
	lineID = -1;
	componentID = -1;
}

bool compute::header::isempty()
{
	return ((messageID == -1) && (itemID == -1) && (lineID == -1) && (componentID == -1));
}

string compute::header::serialize()
{
	string result = string("{\"header\":{\"m\":");
	result += string::fromInt(messageID);
	result += string(",\"i\":");
	result += string::fromInt(itemID);
	result += string(",\"l\":");
	result += string::fromInt(lineID);
	result += string(",\"c\":");
	result += string::fromInt(componentID);
	result += string(",");
	result += start.to();
	result += string(",");
	result += end.to();
	result += string("} }");

	return result;
}

void compute::header::copy(header const &source)
{
	messageID = source.messageID;
	itemID = source.itemID;
	lineID = source.lineID;
	componentID = source.componentID;
	
	zone::copy(source);
}