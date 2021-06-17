#include "parameters.h"

void web::parameters::reset()
{
	init = false; cleanup();

	length = 0UL;

	headers = new web::parameter*[MAX];
	if (headers == NULL) return;
	for (unsigned long i = 0UL; i < MAX; ++i) headers[i] = NULL;

	for (unsigned long i = 0UL; i < MAX; ++i)
	{
		headers[i] = new web::parameter();
		if (headers[i] == NULL) return;
	}

	clear();

	init = true;
}

void web::parameters::clear() 
{ 
	length = 0UL;
	map.clear();
	//return headers.clear(); 
}

bool web::parameters::add(parameter &source)
{
	if (count() >= MAX) return false;

	//if (map.find(source.name) == map.end()) map[source.name] = (int)headers.size();
	//headers.push_back(source);

	if (map.find(source.name) == map.end()) map[source.name] = (int)length;
	*headers[length++] = source;

	return true;
}

bool web::parameters::exists(string &name)
{
	return (map.find(name) != map.end());
}

web::parameter& web::parameters::get(unsigned long index)
{
	return *headers[index];
}

string web::parameters::get(string &name)
{
	if (map.find(name) != map.end()) return headers[map[name]]->value;

	return string("");
}

long web::parameters::count() 
{ 
	//return (unsigned long)headers.size(); 
	return (long)length;
}

void web::parameters::makeNull() 
{ 
	headers = NULL;
}

void web::parameters::cleanup() 
{
	if (headers != NULL)
	{
		for (long i = (MAX - 1L); i >= 0L; i--)
		{
			if (headers[i] != NULL) delete headers[i];
		}

		delete headers;
	}
}