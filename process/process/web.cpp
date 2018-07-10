#include "web.h"
#include "log.h"

void web::address::reset()
{
	port = 80; secure = false;

	server.reset(); path.reset();
}

bool web::address::get(string &url)
{
	secure = false;
	long count = 0L, index = url.indexOf(string("http://"));
	if (index < 0L)
	{
		index = url.indexOf(string("https://"));
		if (index < 0L)
		{
			index = 0L;
		}
		else
		{
			secure = true;
		}
	}

	if (index == 0L)
	{
		index = 7L;
		if (secure)
		{
			index = 8L;
		}
	}
	else if (index != 0L) return false;

	if (secure) port = 443;
	else port = 80;

	server.reset();
	path.reset();

	do
	{
		char character = url[index];

		if (character == '/') ++count;

		if (count == 0) server.concat(&character, 1L);
		else path.concat(&character, 1L);

		++index;
	} while (index < url.count());

	if (path.count() == 0L) path.copy(string("/"));

	return ((path.count() > 0L) && (server.count() > 0L));
}

void web::page::reset()
{
	init = false; cleanup();
	status = 0L;

	parameters = new list<parameter>();
	if (parameters == NULL) return;

	body = new string(length);
	if (body == NULL) return;

	init = true;
}

bool web::page::set(string &name, string &value)
{
	return parameters->add(parameter(name, value));
}

string web::page::get(string &name)
{
	for (long i = 0L; i < (long)parameters->count(); ++i)
	{
		parameter t = (*parameters)[i];
		if (t.name == name) return t.value;
	}

	return string();
}

void web::page::makeNull()
{
	parameters = NULL;
	body = NULL;
}

void web::page::cleanup()
{
	if (body != NULL) delete body;
	if (parameters != NULL) delete parameters;
}
