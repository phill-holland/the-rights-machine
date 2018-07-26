#include "address.h"

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
