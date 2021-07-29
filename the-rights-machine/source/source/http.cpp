#include "http.h"
#include "log.h"
#include <string.h>
#include <math.h>
#include <openssl/rand.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

bool http::client::client::get(web::page *destination, 
							   web::page *source, 
							   web::parameter *parameters, 
							   long count)
{
	if (destination == NULL) return false;
	destination->clear();

	web::address addr(destination->url);
	addr.port = destination->port;

	return issue(string("GET"), addr, destination, source, parameters, count);
}

bool http::client::client::post(web::page *destination, 
								web::page *source, 
								web::parameter *parameters, 
								long count)
{
	if (destination == NULL) return false;
	destination->clear();

	web::address addr(destination->url);
	addr.port = destination->port;

	return issue(string("POST"), addr, destination, source, parameters, count);
}

bool http::client::client::head(web::page *destination)
{
	if (destination == NULL) return false;

	web::address addr(destination->url);
	addr.port = destination->port;

	return issue(string("HEAD"), addr, destination, NULL, NULL, 0L);
}

bool http::client::client::issue(string command,
								 web::address &addr,
								 web::page *destination,
								 web::page *source,
								 web::parameter *parameters,
								 long count)
{
	if (!::wsock::client::open(addr.server, addr.port)) return false;
	bool result = false;
	string request;

	request.concat(command);

	request.concat(string(" "));
	request.concat(addr.path);
	request.concat(string(" HTTP/1.1\r\n"));

	request.concat(string("Host: "));
	request.concat(string(addr.server));
	request.concat(string("\r\n"));

	if (parameters != NULL)
	{
		for (long i = 0L; i < count; ++i)
		{
			request.concat(parameters[i].name);
			request.concat(string(": "));
			request.concat(parameters[i].value);
			request.concat(string("\r\n"));
		}
	}

	if (destination->authorization.required)
	{
		string auth = string(destination->authorization.username);
		auth.concat(string(":"));
		auth.concat(destination->authorization.password);

		request.concat(string("Authorization: Basic "));
		request.concat(auth.toBase64());
		request.concat(string("\r\n"));
	}

	request.concat(string("Cache-Control: no-cache\r\n"));

	if (source != NULL)
	{
		for (long i = 0L; i < source->parameters.count(); ++i)
		{
			request.concat(source->parameters[i].name);
			request.concat(string(": "));
			request.concat(source->parameters[i].value);
			request.concat(string("\r\n"));
		}

		if (source->body->size() > 0L)
		{
			request.concat(string("Content-Length: " + string::fromLong((long)source->body->size()) + string("\r\n")));
		}
	}

	request.concat(string("\r\n"));

	if (source != NULL) request.concat(*(source->body));

	bool ok = true;
	if (addr.secure)
	{
		if (!ssl::open(_socket)) return false;
		if (!ssl::write(request)) ok = false;
	}
	else
	{
		//if (::wsock::client::write(request, 0) == SOCKET_ERROR) ok = false;
		if (::wsock::client::write(request, 0) == 0) ok = false;
	}

	if (ok)
	{
		long running_total = 0L, actual_total = 0L;
		int t, error = 0, state = 0;
		char character;
		bool finished = false;

		string name, value;
		string status;
		bool isValue = false;
		long line = 0L, space = 0L;

		do
		{
			if (!addr.secure)t = ::wsock::client::read(&character, 1, 0);
			else t = ssl::read(&character, 1L);

			if (t > 0)
			{
				if (character == ' ') ++space;

				if ((character != 13) && (character != 10) && (character != ':') && (character != ' '))
				{
					if (line == 0L)
					{
						if (space == 1L) status.concat((char *)&character, 1L);
					}
					else
					{
						if (!isValue) name.concat((char *)&character, 1L);
						else value.concat((char *)&character, 1L);
					}
				}

				if ((character == 13) && (state == 0)) ++state;
				else if ((character == 10) && (state == 1))
				{
					if ((name.count() > 0) && (value.count() > 0)) destination->set(name, value);
					name.reset(); value.reset();
					isValue = false;
					space = 0L;

					++line;
					++state;
				}
				else if ((character == 13) && (state == 2)) ++state;
				else if ((character == 10) && (state == 3)) finished = true;
				else
				{
					if (character == ':') isValue = true;
					state = 0;
				}
			}
			else ++error;

		} while ((t > 0) && (error < 10) && (!finished));

		destination->status = status.toLong();

		if ((error < 10) && (finished))
		{
			long length = destination->get(string("Content-Length")).toLong();
			if (length > 0L)
			{
				running_total = length;
				long remaining = length;
				do
				{
					long bufsize = receive_length;
					if (remaining < receive_length) bufsize = remaining;

					if (!addr.secure) t = ::wsock::client::read(receive, bufsize, 0);
					else t = ssl::read(receive, bufsize);

					if (t > 0)
					{
						actual_total += (long)t;
						remaining -= (long)t;
						destination->body->concat(receive, t);
					}
					else ++error;
				} while ((t > 0) && (error < 10) && (remaining > 0L));
			}
			else
			{
				string encoding = destination->get(string("Transfer-Encoding"));
				if (encoding.isIn(string("chunked")))
				{
					bool data = false, finished = false;
					char previous = 0;
					long bytestoread = 0L;
					//bool no_more_data = false;

					//do
					//{
					do
					{
						string chunk;
						t = 0;
						error = 0;
						state = 0;
						finished = false;
						do
						{
							if (!addr.secure) t = ::wsock::client::read(&character, 1, 0);
							else t = ssl::read(&character, 1L);

							if (t > 0)
							{
								if ((character != 13) && (character != 10))
									chunk.concat(&character, 1L);

								if ((character == 13) && (state == 0)) ++state;
								else if ((character == 10) && (state == 1))
								{
									if (chunk.count() > 0L) finished = true;
									else state = 0;
								}
								else state = 0;
							}
							else ++error;
						} while ((t > 0) && (error < 10) && (!finished));

						if(t > 0) //&& (character != 10) && (character != 13))
						{
							// need state machine for proper chunking
							// cr lf reading

							//if(((character != 10) && character != 13))
							//{
								bytestoread = chunk.toLongFromHex();
								//std::cout << "bytses " << bytestoread << "\n";
								//now read cr lf here, before reading the data
								if (bytestoread > 0L)
								{
									// ***
									//if (!addr.secure) t = ::wsock::client::read(receive, 2, 0);
									//else t = ssl::read(receive, 2);

									//std::cout << "[" << string(receive,2) << "]\n";
									// ***
									running_total += bytestoread;
									long remaining = bytestoread;
									error = 0; t = 0;
									do
									{
										long bufsize = receive_length;
										if (remaining < receive_length) bufsize = remaining;

										if (!addr.secure) t = ::wsock::client::read(receive, bufsize, 0);
										else t = ssl::read(receive, bufsize);

										if (t > 0)
										{
											actual_total += (long)t;
											remaining -= (long)t;
											destination->body->concat(receive, t);
										}
										else ++error;
									} while ((t > 0) && (remaining > 0) && (error < 10));

									//std::cout << "t " << t << " remaining " << remaining << " eror " << error << "\n";
									//std::cout << "receiv-leng " << receive_length << "\n";
									//if (!addr.secure) t = ::wsock::client::read(receive, 2, 0);
									//else t = ssl::read(receive, 2L);

									//std::cout << "plop[" << string(receive,2) << "]\n";
								}
							//}
						} else bytestoread = 0L;
						
					} while (bytestoread > 0L);

					// trailer CRLF
					//if (!addr.secure) t = ::wsock::client::read(receive, 2, 0);
					//else t = ssl::read(receive, 2L);

					//std::cout << "plop[" << string(receive,2) << "]\n";
				}
			}
		}

		if ((destination->status == 301L) || (destination->status == 302L) || (destination->status == 307L) || (destination->status == 308L))
			destination->redirect = destination->get(string("Location"));

		result = ((destination->status == 200) && (running_total == actual_total));
	}

	if (addr.secure) ssl::close();
	::wsock::client::close();
	
	return result;
}
