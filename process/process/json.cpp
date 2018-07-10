#include "json.h"
#include "log.h"

string data::json::FQDN()
{
	string result = identifier();

	//Log << "I AM " << result << "\r\n";

	json *current = _parent;
	while (current != NULL)	 // NEXT ESCAPE COUNT, CANNOT EXCEED MAX DEPTH ..????
	{
	//	Log << "CURRENT " << current->identifier() << "\r\n";
	//	if (current->_parent != NULL) Log << "MY PARENT " << current->_parent->identifier() << "\r\n";

		if(current->identifier().count() > 0L) result = current->identifier() + "\\" + result;
		current = current->_parent;
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