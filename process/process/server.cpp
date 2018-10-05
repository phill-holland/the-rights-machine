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
				else if ((receiving[i] == 10) && (h_index == 3)) 
				{ 
					// ****
					validate();
					// ****

					header = false; 
				}
				else h_index = 0;
			}
			else
			{
				// need to check for double \r\n
				if (read_counter++ >= content_length - 2)
				{
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
							string t = (string)label;

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
									data::user user = c->configuration.users->get(task.message.user);
									if (!user.isempty())
									{
										if(user.validate(task.message))
										{
											guid::guid g;
											task.message.guid = g.get();
											task.message.created = global::datetime::now();

											if (!c->configuration.manager->set(task))
											{
												error(string("MESSAGE_PUSH"));
											}
											else
											{
												if (c->configuration.requested != NULL)
												{
													if (!c->configuration.requested->add(::pending::waiting(task.message.guid, task.message.user)))
													{
														error(string("ALREADY_REQUESTED"));
													}
												}

												::data::response::response response;

												response.guid = task.message.guid;
												response.created = datetime::now();
												response.available = false;

												outputter.set(&response);
											}
										} 
										else error(string("INVALID_APIKEY"));

									}
									else error(string("INVALID_USER"));
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
										if (requested.guid.count() > 0L)
										{
											data::response::response result = task.response->find(requested.guid);											
											if(result.validate(requested))
											{
												if (c->configuration.requested != NULL)
												{
													if (!c->configuration.requested->remove(::pending::waiting(requested.guid, requested.user))) error(string("NOT_IN_PENDING"));
												}

												outputter.set(&result);
											}
											else
											{
												data::response::response::STATUS status = data::response::response::STATUS::UNKNOWN;

												if (c->configuration.requested != NULL)
												{
													if (c->configuration.requested->contains(::pending::waiting(requested.guid, requested.user))) status = data::response::response::STATUS::PENDING;
													else error(string("NOT_IN_PENDING"));
												}

												result.clear();

												result.guid = requested.guid;
												result.user = requested.user;
												result.status = status;
												
												outputter.set(&result);
												// NEED TO ADD STATUS PENDING, IF WAITING FOR OBJECT

												// push not found back to user (create hash lookup for valid GUI'IDs)
												// check hash that GUI was pushed through to be processed

												// create output queue for pointers for json::response *
												// hmmm, but need to store the actual objects somewhere??
											}
										}
										else
										{
											Log << "no guid found\r\n";
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

		if ((bytes <= 0) && (++errors >= ERRORS))
		{
			if (read_counter >= content_length - 2)
			{
				if (get() != MODE::NONE)
				{
					goodbye();
					//Log << "Client terminated connection, expected\r\n";
				}
			}
			else error(string("READ"));
		}
	}
	
	return (DWORD)0;
}

void server::listener::reset(client *source)
{
	init = false;

	c = source; 

	task.response = c->configuration.responses;
	if (task.response == NULL) return;

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

	parents.clear();

	// clear data classes - lines/items/components/elements/queries etc..
}

server::listener::MODE server::listener::get() 
{ 
	if(command.compare(string("POST"))) return MODE::POST; 
	else if (command.compare(string("GET"))) return MODE::GET;

	return MODE::NONE;
}

void server::listener::validate()
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

	// check content_length isn't zero, isn't too big, isn't mismatch with the data
	// check command is either POST or GET

	brackets = 0;
	squares = 0;

	quotes = false;
	left = true;

	outputter.clear();
	
	//validate = false;
}

void server::listener::goodbye()
{
	c->goodbye();
	clear();
}

void server::listener::error(string &error)
{
	// output error via JSON/response
	//Log << "erroring " << error << "\r\n";
	//c->configuration.errors->set(::error::error(error));
	//c->makeError(client::ERRORS::Read);
	c->makeError(::error::error(error));
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

void server::client::makeError(::error::error &error)//ERRORS code)
{
	configuration.errors->set(error);
	lastErrorCode = error;
	isInError = true;
}

void server::client::resetError()
{
	isInError = false;
	lastErrorCode = ::error::error(string("NONE"));//ERRORS::None;
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

	return (DWORD)0;
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

void server::server::output(string &source)
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