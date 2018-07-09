#include "json.h"

string data::json::FQDN()
{
	string result = identifier();

	json *current = _parent;
	while (current != NULL)
	{
		if(current->identifier().count() > 0L) result = current->identifier() + "\\" + result;
		current = _parent->_parent;
	};
	
	return result;
}

/*
bool data::json::walk(string *parents, long depth)
{
	// might use hashset to remap parents to numbers!!
	// to make walking quicker??
	json *p = _parent;
	for (long i = (depth - 1L); i > 0L; i--)
	{
		if (p != NULL)
		{
			if (!parents[i].icompare(p->identifier())) return false;
			p = p->_parent;
		}
		else return false;
	}

	return true;
}
*/