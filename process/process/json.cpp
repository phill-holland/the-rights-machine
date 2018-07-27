#include "json.h"
#include "log.h"

string data::json::request::json::FQDN()
{
	string result = identifier();

	json *current = _parent;
	while (current != NULL)	
	{	
		if(current->identifier().count() > 0L) result = current->identifier() + "\\" + result;
		current = current->_parent;
	};
	
	return result;
}