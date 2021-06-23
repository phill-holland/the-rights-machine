#include "server.h"
#include "task.h"
#include "guid.h"
#include "log.h"

void server::listener::background(thread *bt)
{
	sleep(10);

	if ((c->isopen()) && (!c->isError()))
	{
		int bytes = c->read(receiving, RECEIVING, 0);
		int index = 0, temp = 0;

		if(request)
		{
			request = intent(receiving, bytes, index);
			if(!request) header = true;
		}

		if(header) 
		{
			header = heading(&receiving[index], bytes - index - 1, temp);
			if(!header) 
			{
				read_counter = 0;
				outputter.clear(); // THIS IS THE FUNCTION TO RETURN DATA TO CALLING CLIENT
				// PASS THIS INTO compute::Task
				if(!validate()) error(string("HEADER"));

				// TASK.RESPONSE, overload with different type of output!!
				// i.e. as a wrapper for outputter
			}
			index += temp;
		}

		if((!header)&&(!request))
		{
			int read = bytes - index - 1;			
			int n = parser->write(&receiving[index], read, ec);
			read_counter += n;
			if(n != read) ++errors;
		}

		if ((bytes <= 0) && (++errors >= ERRORS))
		{
			if (read_counter >= content_length - 2)
			{
				if (get() != MODE::NONE)
				{
					// GOODBYE STATE IS NOW WHEN FULL RESULTS RETURNED
					goodbye();
				}
			}
			else error(string("READ"));
		}
	}
}

void server::listener::reset(client *source)
{
	cleanup(); init = false;

	c = source;

	if(c->configuration.responses == NULL) return;
	if(c->configuration.manager == NULL) return;

	parser = new parser::parser(c->configuration.manager, c->configuration.responses);
	if(parser == NULL) return;
	
	clear();

	init = true;
}

void server::listener::clear()
{
	parameters.clear();
	parser->clear();

	header = false;
	request = true;

	h_index = 0;

	errors = 0L;

	left = true;

	label.clear();
	value.clear();
	command.clear();

	memset(receiving, 0, RECEIVING);

	// clear data classes - lines/items/components/elements/queries etc..
}

server::listener::MODE server::listener::get()
{
	if(command.compare(string("POST"))) return MODE::POST;
	else if (command.compare(string("GET"))) return MODE::GET;

	return MODE::NONE;
}

bool server::listener::validate()
{
	//get first line of HTTP GET function - to determine which function was called
	//determine if get or post!!!

	//get host - ip address, log
	//get content - type check application / json
	//get content - length
	//check http1.1

	string length = parameters.get(string("Content-Length"));
	content_length = length.toLong();
	//read_counter = 0L;

	// check content_length isn't zero, isn't too big, isn't mismatch with the data
	// check command is either POST or GET

	left = true;
//	outputter.clear();

	if(get()==MODE::NONE) return false;

	return content_length > 0;

	//validate = false;
}

bool server::listener::intent(char *source, int length, int &index)
{
	errors = 0L;
	for (int i = 0; i < length; ++i)
	{
		//errors = 0L;

		if (request)
		{
			if(!command.push(receiving[i]))
			{
				error(string("STRING_TOO_LONG"));
			}

			if (receiving[i] == 13) ++h_index;
			else if ((receiving[i] == 10) && (h_index == 1))
			{
				h_index = 0;
				index = i + 1;
				return false;
				//request = false;
				//header = true;
			}
		}
	}

	return true;
}

bool server::listener::heading(char *source, int length, int &index)
{
	errors = 0L;
	for (int i = 0; i < length; ++i)
	{
		//errors = 0L;
		char in = source[i];

		if (in== ':')
		{
			left = false;
		}
		else if ((in != 13) && (in != 10) && (in != ' '))
		{
			if (left)
			{
				if(!label.push(in))
				{
					error(string("STRING_TOO_LONG"));
				}
			}
			else
			{
				if(!value.push(in))
				{
					error(string("STRING_TOO_LONG"));
				}
			}
		}

		if ((in == 13) || (in == 10))
		{
			if(!label.isempty() || !value.isempty())
			{
				parameters.add(web::parameter((string)label, (string)value));
			}

			left = true;

			label.clear();
			value.clear();
		}

		if (in == 13) ++h_index;
		else if ((in == 10) && (h_index == 1)) ++h_index;
		else if ((in == 13) && (h_index == 2)) ++h_index;
		else if ((in == 10) && (h_index == 3))
		{
			//validate();
			index = i + 1;		
			return false;
		}
		else h_index = 0;
	}

	return true;
}

void server::listener::goodbye()
{
	c->goodbye();
	clear();
}

void server::listener::error(string error)
{
	::error::error err(error);
	c->makeError(err);

	outputter.clear();
	::error::type::type type = c->configuration.errors->lookup(err);
	outputter.set(&type);
	string temp = outputter.get();
	c->write(temp, 0);

	clear();
}

void server::client::states::reset()
{
	for (long i = 0L; i < MAX; ++i) children[i].reset();
}

bool server::client::states::isAnyIdle()
{
	for (long i = 0L; i < MAX; ++i)
	{
		if ((children[i].state == pending::STATE::Idle) && (children[i].identity == 0L))
		{
			return true;
		}
	}

	return false;
}

bool server::client::states::isAnyReady()
{
	for (long i = 0L; i < MAX; ++i)
	{
		if ((children[i].state == pending::STATE::Ready) && (children[i].identity == 0L))
		{
			return true;
		}
	}

	return false;
}

bool server::client::states::isAnyPending()
{
	for (long i = 0L; i < MAX; ++i)
	{
		if ((children[i].state == pending::STATE::Pending) && (children[i].identity > 0L))
		{
			return true;
		}
	}

	return false;
}

bool server::client::states::isPending(long identity)
{
	for (long i = 0L; i < MAX; ++i)
	{
		if ((children[i].state == pending::STATE::Pending) && (children[i].identity == identity))
		{
			return true;
		}
	}

	return false;
}

bool server::client::states::isAnyFinished()
{
	for (long i = 0L; i < MAX; ++i)
	{
		if ((children[i].state == pending::STATE::Finished) && (children[i].identity > 0))
		{
			return true;
		}
	}

	return false;
}

bool server::client::states::setIdleToReady()
{
	for (long i = 0L; i < MAX; ++i)
	{
		if ((children[i].state == pending::STATE::Idle) && (children[i].identity == 0L))
		{
			children[i].state = pending::STATE::Ready;
			return true;
		}
	}

	return false;
}

bool server::client::states::setReadyToPending(long identity)
{
	for (long i = 0L; i < MAX; ++i)
	{
		if ((children[i].state == pending::STATE::Ready) && (children[i].identity == 0L))
		{
			children[i].state = pending::STATE::Pending;
			children[i].identity = identity;

			return true;
		}
	}

	return false;
}

bool server::client::states::setPendingToFinished(long identity)
{
	for (long i = 0L; i < MAX; ++i)
	{
		if ((children[i].state == pending::STATE::Pending) && (children[i].identity == identity))
		{
			children[i].state = pending::STATE::Finished;

			return true;
		}
	}

	return false;
}

bool server::client::states::setFinishedToIdle(long identity)
{
	for (long i = 0L; i < MAX; ++i)
	{
		if ((children[i].state == pending::STATE::Finished) && (children[i].identity == identity))
		{
			children[i].state = pending::STATE::Idle;
			children[i].identity = 0L;

			return true;
		}
	}

	return false;
}

void server::client::states::output()
{
	string result = string("\r\nServer) ");
	for (long i = 0L; i < MAX; ++i)
	{
		result.concat("[(");
		result.concat(string::fromInt(i));
		result.concat(") ");
		if (children[i].isIdle()) result.concat("Idle");
		else if (children[i].isPending()) result.concat("Pending");
		else if (children[i].isReady()) result.concat("Ready");
		else if (children[i].isFinished()) result.concat("Finished");
		result.concat("]");
	}
	result.concat("\r\n");
	Log << result;
}

void server::client::reset(configuration::server::client::configuration &configuration)
{
	init = false; cleanup();

	this->configuration = configuration;

	//isInExit = false;

	//isInError = false;
	//lastErrorCode = ::error::error(string("NONE"));//ERRORS::None;

	listen = new listener(this);
	if (listen == NULL) return;

	clear();

	init = true;
}

void server::client::clear()
{
	statuses.reset();
	resetError();
	resetExit();
}

bool server::client::start()
{
	if (listen == NULL) return false;
	listen->start();
	if (!listen->initalised()) return false;

	return true;
}

bool server::client::stopAndWait()
{
	bool result = true;

	if (listen != NULL) { if (!listen->stopAndWait()) result = false; }

	return result;
}

bool server::client::pauseAndWait(bool value)
{
	if (listen != NULL) if (!listen->pauseAndWait(value)) return false;

	return true;
}

bool server::client::isPaused()
{
	return listen->isPaused();
}

void server::client::goodbye()
{
	isInExit = true;
}

bool server::client::isDeparting()
{
	return isInExit;
}

void server::client::resetExit()
{
	isInExit = false;
}

bool server::client::isError()
{
	return isInError;
}

void server::client::makeError(::error::error &error)
{
	configuration.errors->set(error);
	lastErrorCode = error;
	isInError = true;
}

void server::client::resetError()
{
	isInError = false;
	lastErrorCode = ::error::error(string("NONE"));
}

void server::client::shutdown()
{
	stopAndWait();
	close();
}

void server::client::makeNull()
{
	listen = NULL;
}

void server::client::cleanup()
{
	if (listen != NULL) delete listen;
}

void server::wait::background(thread *bt)
{
	sleep(250);

	client *c = s->findUnconnectedClient();
	if (c != NULL)
	{
		if (s->wait(*c))
		{
			c->start();
		}
	}

	//return (DWORD)0;
}

void server::watchdog::background(thread *bt)
{
	sleep(250);

	mutex lock(s->token);

	for (long i = 0; i < s->configuration.clients; ++i)
	{
		if (s->clients[i]->isError())
		{
			::error::error lastError = s->clients[i]->lastError();

			s->output(string("Server detected client [") << i << "] in error(" << (lastError.name) << "), shutting down");

			s->clients[i]->shutdown();
			s->clients[i]->clear();
		}
		else if (s->clients[i]->isDeparting())
		{
			s->output(string("Server detected client [") << i << "] in graceful shutdown");

			s->clients[i]->shutdown();
			s->clients[i]->clear();
		}
	}

	//return (DWORD)0;
}

void server::server::reset(configuration::server::configuration *settings)
{
	init = false; cleanup();
	iterations = 0L; counter = 0L;

	configuration = *settings;

	requested = new ::pending::pending();
	if (requested == NULL) return;
	if (!requested->initalised()) return;

	configuration::server::client::configuration temp(*settings, requested);

	clients = new client*[configuration.clients];
	if (clients == NULL) return;
	for (long i = 0L; i < configuration.clients; ++i) clients[i] = NULL;

	for (long i = 0L; i < configuration.clients; ++i)
	{
		clients[i] = new client(temp);
		if (clients[i] == NULL) return;
		if (!clients[i]->initalised()) return;
	}

	waiter = new ::server::wait(this);
	if (waiter == NULL) return;

	watcher = new ::server::watchdog(this);
	if (watcher == NULL) return;

	init = true;
}

bool server::server::start()
{
	if ((waiter == NULL) || (watcher == NULL)) return false;

	waiter->start();
	if (!waiter->initalised()) return false;

	watcher->start();
	if (!watcher->initalised()) return false;

	return true;
}

void server::server::stop()
{
	if (watcher != NULL) watcher->stop();
	if (waiter != NULL) waiter->stop();

	if (clients != NULL)
	{
		for (long i = 0L; i < configuration.clients; ++i)
		{
			if (clients[i] != NULL) clients[i]->stopAndWait();
		}
	}
}

bool server::server::pauseAndWait(bool value)
{
	if (watcher != NULL) if (!watcher->pauseAndWait(value)) return false;
	if (waiter != NULL) if (!waiter->pauseAndWait(value)) return false;

	return true;
}

void server::server::shutdown()
{
	stop();
	close();

	if (clients != NULL)
	{
		for (long i = 0L; i < configuration.clients; ++i)
		{
			if (clients[i] != NULL)
			{
				if (clients[i]->isopen()) clients[i]->shutdown();
			}
		}
	}
}

bool server::server::finisherPauseAndWait(bool value)
{
	if (watcher != NULL) if (!watcher->pauseAndWait(value)) return false;
	if (waiter != NULL) if (!waiter->pauseAndWait(value)) return false;

	return true;
}

server::client *server::server::findUnconnectedClient()
{
	if (clients != NULL)
	{
		for (long i = 0L; i < configuration.clients; ++i)
		{
			if (!clients[i]->isopen())
			{
				return clients[i];
			}
		}
	}

	return NULL;
}

server::client *server::server::findConnectedIdleClient(long &index)
{
	if (clients != NULL)
	{
		index = 0L;

		for (long i = 0L; i < configuration.clients; ++i)
		{
			if (clients[i]->isopen())
			{
				if (clients[i]->statuses.isAnyIdle())
				{
					index = i;
					return clients[i];
				}
			}
		}
	}

	return NULL;
}

server::client *server::server::findConnectedReadyClient(long &index)
{
	if (clients != NULL)
	{
		index = 0L;

		for (long i = 0L; i < configuration.clients; ++i)
		{
			if (clients[i]->isopen())
			{
				if (clients[i]->statuses.isAnyReady())
				{
					index = i;
					return clients[i];
				}
			}
		}
	}

	return NULL;
}

server::client *server::server::findCompletedClient(long &index)
{
	if (clients != NULL)
	{
		index = 0L;

		for (long i = 0L; i < configuration.clients; ++i)
		{
			if (clients[i]->isopen())
			{
				if (clients[i]->statuses.isAnyFinished())
				{
					index = i;
					return clients[i];
				}
			}
		}
	}

	return NULL;
}

void server::server::output(string source)
{
	//if (!config.headless) terminal->set(source);
	//else Log << source << "\r\n";

	Log << source << "\r\n";
}

void server::server::makeNull()
{
	requested = NULL;
	clients = NULL;
	waiter = NULL;
	watcher = NULL;
}

void server::server::cleanup()
{
	shutdown();

	if (watcher != NULL) delete watcher;
	if (waiter != NULL) delete waiter;
	if (clients != NULL)
	{
		for (long i = (configuration.clients - 1L); i >= 0L; i--)
		{
			if (clients[i] != NULL) delete clients[i];
		}

		delete clients;
	}
	if (requested != NULL) delete requested;
}