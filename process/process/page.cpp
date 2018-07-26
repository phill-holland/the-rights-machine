#include "page.h"

void web::page::reset()
{
	init = false; cleanup();
	status = 0L;

	body = new string(length);
	if (body == NULL) return;

	parameters.clear();

	init = true;
}

bool web::page::set(string &name, string &value)
{
	return parameters.add(parameter(name, value));
}

string web::page::get(string &name)
{
	for (long i = 0L; i < (long)parameters.count(); ++i)
	{
		parameter t = parameters[i];
		if (t.name == name) return t.value;
	}

	return string();
}

void web::page::makeNull()
{
	body = NULL;
}

void web::page::cleanup()
{
	if (body != NULL) delete body;
}
