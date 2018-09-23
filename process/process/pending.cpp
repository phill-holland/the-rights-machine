#include "pending.h"

void pending::pending::reset()
{
	init = false; cleanup();

	map.clear();

	init = true;
}

bool pending::pending::add(::pending::waiting wait)
{
	if (!wait.validate()) return false;

	std::unordered_map<string, waiting, hasher, equality>::iterator i = map.find(wait.guid);
	if (i != map.end()) return false;

	map[wait.guid] = wait;

	return true;
}

bool pending::pending::remove(::pending::waiting &wait)
{
	std::unordered_map<string, waiting, hasher, equality>::iterator i = map.find(wait.guid);
	if (i != map.end())
	{
		waiting temp = (waiting)i->second;

		if (temp.user.icompare(wait.user))
		{
			map.erase(wait.user);

			return true;
		}
	}

	return false;
}

bool pending::pending::contains(::pending::waiting &wait)
{
	std::unordered_map<string, waiting, equality>::iterator i = map.find(wait.guid);
	if (i == map.end()) return false;

	waiting temp = map[wait.guid];

	return temp.user.icompare(wait.user);
}

void pending::pending::makeNull() 
{
}
	
void pending::pending::cleanup() 
{ 
}