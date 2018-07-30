#include "server.h"
#include "task.h"
#include "guid.h"
#include "log.h"

DWORD WINAPI server::listener::background(thread *bt)
{
	Sleep(10);

	if ((c->isopen()) && (!c->isError()))
	{
		int bytes = c->read(receiving, RECEIVING, 0);
		for (int i = 0; i < bytes; ++i)
		{
			errors = 0L;

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

					request = false;
					header = true;
				}
			}
			else if (header)
			{
				if (receiving[i] == ':')
				{
					left = false;
				}
				else if ((receiving[i] != 13) && (receiving[i] != 10) && (receiving[i] != ' '))
				{
					if (left)
					{
						if(!label.push(receiving[i]))
						{
							error(string("STRING_TOO_LONG"));
						}
					}
					else
					{
						if(!value.push(receiving[i]))
						{
							error(string("STRING_TOO_LONG"));
						}
					}
				}

				if ((receiving[i] == 13) || (receiving[i] == 10))
				{
					if(!label.isempty() || !value.isempty())
					{
						parameters.add(web::parameter((string)label, (string)value));
					}

					left = true;

					label.clear();
					value.clear();
				}

				if (receiving[i] == 13) ++h_index;
				else if ((receiving[i] == 10) && (h_index == 1)) ++h_index;
				else if ((receiving[i] == 13) && (h_index == 2)) ++h_index;
				else if ((receiving[i] == 10) && (h_index == 3)) { validate = true; header = false; }
				else h_index = 0;
			}
			else if (validate)
			{
				//get first line of HTTP GET function - to determine which function was called
					//determine if get or post!!!

					//get host - ip address, log
					//get content - type check application / json
					//get content - length
					//check http1.1
				string length = parameters.get(string("Content-Length"));
				content_length = length.toLong();
				read_counter = 0L;

				outputter.clear();

				validate = false;
			}
			else
			{
				if (read_counter++ >= content_length)
				{
					Log << "read done\r\n";
					c->write(outputter.get(), 0);
					
					// send response
					// kill connection
				}

				if (receiving[i] == '"')
				{
					quotes = !quotes;
				}
				else
				{
					if (!quotes)
					{
						if (receiving[i] == '{')
						{
							if (!parents.push((string)label))
							{
								error(string("NESTING_TOO_DEEP"));
							}

							label.clear();
							value.clear();

							left = true;

							++brackets;
						}
						else if (receiving[i] == '}')
						{
							if (get() == MODE::POST)
							{
								if (!parents.isempty())
								{
									::data::json::request::json *current = task.message.find(parents.FQDN());
									if (current != NULL) current->add(custom::pair(label, value));
								}

								queue::base *b = task.message.findQ(parents.FQDN());
								if (b != NULL) b->flush();

								if (parents.FQDN().icompare(task.message.items.FQDN()))
								{
									Log << "PUSH MESSAGE TO OUTPUT\r\n";
									Log << "NEED TO WRITE OUTPUT FUNCTION FOR MESSAGE\r\n";

									guid::guid g;
									task.message.GUID = g.get();
									task.message.created = global::datetime::now();
									// PUSH GUID TO UNORDERED_MAP

									task.message.output();

									if (!c->manager->set(task))
									{
										error(string("MESSAGE_PUSH"));
									}
									else
									{
										::data::response::response response;
										response.GUID = task.message.GUID;
										outputter.set(&response);
										// build response list - json classes
										// when content-length is read, write response
									}
								}
							}
							else if (get() == MODE::GET)
							{
								if (!parents.isempty())
								{
									::data::json::request::json *current = requested.find(parents.FQDN());
									if (current != NULL) current->add(custom::pair(label, value));

									if (parents.FQDN().icompare(requested.FQDN()))
									{
										Log << "REQUEST\r\n";

										
										requested.output();

										data::response::response result = task.response->find(requested.GUID);
										if ((result.GUID == requested.GUID) && (result.userID))
										{
											outputter.set(&result);
											// push response back to user

											// response factory
											// error factory
										}
										else
										{
											// push not found back to user (create hash lookup for valid GUI'IDs)
											// check hash that GUI was pushed through to be processed

											// create output queue for pointers for json::response *
											// hmmm, but need to store the actual objects somewhere??
										}

										// need a special type of task queue
										// push things onto queue, with key
										// have find function to retreive item
										// have queue item with datetime
										// so that responses expire if not picked up

										// validate request
										// check task queue
										// task.response
									}
								}
							}

							left = true;

							label.clear();
							value.clear();

							if (!parents.pop())
							{
								error(string("BRACKET_MISMATCH"));
							}

							--brackets;
							if (brackets < 0)
							{
								error(string("BRACKET_MISMATCH"));
							}
						}
						else if (receiving[i] == '[')
						{
							if (!parents.push(label))
							{
								error(string("NESTING_TOO_DEEP"));
							}

							label.clear();
							value.clear();

							left = true;
							++squares;
						}
						else if (receiving[i] == ']')
						{	
							left = true;

							label.clear();
							value.clear();

							if (!parents.pop())
							{
								error(string("BRACKET_MISMATCH"));
							}

							--squares;
							if (squares < 0)
							{
								error(string("BRACKET_MISMATCH"));
							}
						}
						else if (receiving[i] == ':')
						{
							left = false;
						}
						else if (receiving[i] == ',')
						{
							if (get() == MODE::POST)
							{
								if (!parents.isempty())
								{
									::data::json::request::json *current = task.message.find(parents.FQDN());
									if (current != NULL) current->add(custom::pair(label, value));
								}
							}
							else if (get() == MODE::GET)
							{
								if (!parents.isempty())
								{
									::data::json::request::json *current = requested.find(parents.FQDN());
									if (current != NULL) current->add(custom::pair(label, value));
								}
							}

							left = true;

							label.clear();
							value.clear();
						}
						else if ((receiving[i] >= '0') && (receiving[i] <= '9'))
						{
							if (!left)
							{
								if (!value.push(receiving[i]))
								{
									error(string("STRING_TOO_LONG"));
								}
							}
						}
					}
					else
					{
						if (brackets >= 1)
						{
							if ((quotes && left))
							{
								if(!label.push(receiving[i]))
								{
									error(string("STRING_TOO_LONG"));
								}
							}
							else if ((quotes && !left))
							{
								if(!value.push(receiving[i]))
								{
									error(string("STRING_TOO_LONG"));
								}
							}
						}
					}
				}
			}
		}

		if (bytes <= 0)
		{
			++errors;
			if (errors >= ERRORS)
			{
				error(string("READ"));
				//c->makeError(client::ERRORS::Read);
				//clear();
			}
		}
	}

	return (DWORD)0;
}

void server::listener::reset(client *source)
{
	init = false;

	c = source; 

	if (!c->manager->get(*(task.response))) return;

	clear();

	init = true;
}

void server::listener::clear()
{
	parameters.clear();

	header = false;
	request = true;

	h_index = 0;

	errors = 0L;

	quotes = false;
	left = true;

	brackets = 0L;
	squares = 0L;

	label.clear(); 
	value.clear();
	command.clear();

	memset(receiving, 0, RECEIVING);

	//current = NULL;

	parents.clear();

	// clear data classes - lines/items/components/elements/queries etc..
}

server::listener::MODE server::listener::get() 
{ 
	if(command.compare(string("POST"))) return MODE::POST; 
	else if (command.compare(string("GET"))) return MODE::GET;

	return MODE::NONE;
}

void server::listener::error(string &error)
{
	// output error via JSON/response
	c->errors->set(::error::error(error));
	c->makeError(client::ERRORS::Read);
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

/*
distributed::schema *distributed::server::client::states::getPending(long identity)
{
	for (long i = 0L; i < MAX; ++i)
	{
		if ((children[i].state == pending::STATE::Pending) && (children[i].identity == identity))
		{
			return &(children[i].receiving);
		}
	}

	return NULL;
}
*/
/*
distributed::schema *distributed::server::client::states::getFirstFinished(long &identity)
{
	identity = 0;
	for (long i = 0L; i < MAX; ++i)
	{
		if ((children[i].state == pending::STATE::Finished) && (children[i].identity > 0L))
		{
			identity = children[i].identity;
			return &(children[i].receiving);
		}
	}

	return NULL;
}
*/
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

void server::client::reset(manager::manager *manager, error::errors *errors)
{
	init = false; cleanup();

	this->manager = manager;
	this->errors = errors;

	isInError = false;
	lastErrorCode = ERRORS::None;

	listen = new listener(this);
	if (listen == NULL) return;

	init = true;
}

void server::client::clear()
{
	statuses.reset();
	resetError();
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

bool server::client::isError()
{
	return isInError;
}

void server::client::makeError(ERRORS code)
{
	lastErrorCode = code;
	isInError = true;
}

void server::client::resetError()
{
	isInError = false;
	lastErrorCode = ERRORS::None;
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

DWORD WINAPI server::wait::background(thread *bt)
{
	Sleep(250);

	client *c = s->findUnconnectedClient();
	if (c != NULL)
	{
		if (s->wait(*c))
		{
			c->start();
		}
	}

	return (DWORD)0;
}

DWORD WINAPI server::watchdog::background(thread *bt)
{
	Sleep(250);

	mutex lock(s->token);

	for (long i = 0; i < s->configuration.clients; ++i)
	{
		if (s->clients[i]->isError())
		{
			client::ERRORS lastError = s->clients[i]->lastError();

			s->output(string("Server detected client [") << i << "] in error(" << ((int)lastError) << "), shutting down");

			s->clients[i]->shutdown();
			s->clients[i]->clear();
		}
	}

	return (DWORD)0;
}

void server::server::reset(::server::configuration::configuration *settings)
{
	init = false; cleanup();
	iterations = 0L; counter = 0L;

	configuration = *settings;

	clients = new client*[configuration.clients];
	if (clients == NULL) return;

	for (long i = 0L; i < configuration.clients; ++i)
	{
		clients[i] = new client(configuration.manager, configuration.errors);
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
			if (clients[i]->isopen()) clients[i]->shutdown();
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

void server::server::output(string &source)
{
	//if (!config.headless) terminal->set(source);
	//else Log << source << "\r\n";

	Log << source << "\r\n";
}

void server::server::makeNull()
{
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
		for (long i = 0L; i < configuration.clients; ++i)
		{
			if (clients[i] != NULL) delete clients[i];
		}

		delete clients;
	}
}