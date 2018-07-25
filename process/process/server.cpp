#include "server.h"
//#include "parser.h"
#include "task.h"
#include "log.h"

DWORD WINAPI server::listener::background(thread *bt)
{
	// find HTTP headers, and validate
	// need SSL here


	//char **parents;

	//depth = 0L;
	//int DEPTH = 15;

	//bool quotes = false;
	//bool left = true;

	//int brackets = 0;
	//int squares = 0;

	//char label[255];
	//int index = 0;
	//memset(label, 0, 255);

	//char value[255];
	//int val_index = 0;
	//memset(value, 0, 255);

	Sleep(10);

	if ((c->isopen()) && (!c->isError()))
	{
		int bytes = c->read(receiving, RECEIVING, 0);
		for (int i = 0; i < bytes; ++i)
		{
			errors = 0L;

			if (header)
			{
				if (receiving[i] == 13) ++h_index;
				else if ((receiving[i] == 10) && (h_index == 1)) ++h_index;
				else if ((receiving[i] == 13) && (h_index == 2)) ++h_index;
				else if ((receiving[i] == 10) && (h_index == 3)) header = false;
				else h_index = 0;
			}
			else
			{
				if (receiving[i] == '"')
				{
					quotes = !quotes;
				}
				else // HMMMMM
				{
					// if LABEL == MESSAGE - GENERATE GUID for MESSAGE
					if (!quotes)
					{
						if (receiving[i] == '{')
						{
							// just pull the last non-empty parent
							//if (idx_label > 0)
							//{
								strncpy_s(&parents[depth++][0], LENGTH, label, LENGTH);
								idx_label = 0;
								memset(label, 0, LENGTH);
							//}
							left = true;

							++brackets;

							idx_value = 0;
							memset(value, 0, LENGTH);

							if (depth >= DEPTH)
							{
								// error
							}
						}
						else if (receiving[i] == '}')
						{
							if (depth > 0)
							{
								extract(string(&parents[depth - 1][0]), string(label), string(value));
							}

							// OK 
							queue::base *b = task.message.findQ(FQDN());
							if (b != NULL)
							{
								//Log << "FLUSHING " << FQDN() << "\r\n";
								b->flush();
							}
							
							if (FQDN().icompare(task.message.items.FQDN()))
							{
								Log << "PUSH MESSAGE TO OUTPUT\r\n";
								Log << "NEED TO WRITE OUTPUT FUNCTION FOR MESSAGE\r\n";

								task.message.output();

								//::compute::task t;
								//t.message = &message;
								//t.response = &responses;
								//c->server->config->manager->push(t);
								//c->manager->push(compute::task(&message, &response));
								if (!c->manager->set(task))
								{
									// ERROR
								}
							}

							// SILLY - FLUSH MESSAGE to output QUEUE
							// WHEN items ] reached

							// CHECK FQDN() == message.identifier()
							// if true, message.flush()
							//  ALSO
							// if content-length reached, or CRLF CRLF
							// and not flushed message, flush message anyway
							// WAIT, I don't need to flush the message!!!!

							// ****
							// SAVE VALUE
							// ****
							//component.value.copy(value);
							//if (string(&parents[depth - 1][0]).icompare(element.identifier()))
							//{
								//Log << "PUMP\r\n";
								// push to components
								//Log << "Component [value=" << component.value << "]\r\n";
								//temp_comps.set(component);
							//}

							left = true;
							idx_label = 0;
							idx_value = 0;
							memset(label, 0, LENGTH);
							memset(value, 0, LENGTH);

							// this is wrong, needs to be depth - 1
							memset(&parents[depth - 1][0], 0, LENGTH);

							--depth;
							--brackets;
							if (brackets < 0)
							{
								// error!
							}
							if (depth < 0)
							{
								// error
							}
						}
						else if (receiving[i] == '[')
						{
							strncpy_s(&parents[depth++][0], LENGTH, label, LENGTH);
							idx_label = 0;
							memset(label, 0, LENGTH);
							left = true;
							++squares;

							idx_value = 0;
							memset(value, 0, LENGTH);

							if (depth >= DEPTH)
							{
								// error
							}
						}
						else if (receiving[i] == ']')
						{	
							left = true;
							idx_label = 0;
							idx_value = 0;
							memset(label, 0, LENGTH);
							memset(value, 0, LENGTH);

							memset(&parents[depth - 1][0], 0, LENGTH);

							--squares;
							--depth;
							if (squares < 0)
							{
								// error!
							}

							if (depth < 0)
							{
								// error
								// return json result
							}
						}
						else if (receiving[i] == ':')
						{
							left = false;
							//idx_label = 0;
							//memset(label, 0, LENGTH);
							// label should be in buffer
						}
						else if (receiving[i] == ',')
						{
							if (depth > 0)
							{
								extract(string(&parents[depth - 1][0]), string(label), string(value));
							}
							// ****
							// SAVE VALUE
							// ****
							// save value somewhere
							// what's my parent??

							//component.value.copy(value);

							// need depth array
							// and depth limit
							left = true;
							idx_label = 0;
							idx_value = 0;
							memset(label, 0, LENGTH);
							memset(value, 0, LENGTH);
						}
						else if ((receiving[i] >= '0')&&(receiving[i] <= '9'))//(receiving[i] != ' ') 
						{
							if (!left)
							{
								value[idx_value++] = receiving[i];
								if (idx_value >= LENGTH)
								{
									// error
								}
							}
						}
						// if !quotes and !left, could be an integer
						// or some other value

						// if value > 0 and bracket is odd
						// inside a section
					}
					else
					{
						if (brackets >= 1)
						{
							if ((quotes && left))
							{
								label[idx_label++] = receiving[i];
								if (idx_label >= LENGTH)
								{
									// error

								}
							}
							else if ((quotes && !left))
							{
								value[idx_value++] = receiving[i];
								if (idx_value >= LENGTH)
								{
									// error
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
				c->makeError(client::ERRORS::Read);
				clear();
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
	header = true;
	h_index = 0;

	errors = 0L;

	depth = 0L;

	quotes = false;
	left = true;

	brackets = 0L;
	squares = 0L;

	idx_label = 0L;
	idx_value = 0L;

	memset(receiving, 0, RECEIVING);
	memset(label, 0, LENGTH);
	memset(value, 0, LENGTH);

	// CLEAR DATA CLASSES
	current = NULL;

	for (long i = 0L; i < DEPTH; ++i)
	{
		memset(&parents[i][0], 0, LENGTH);
	}
}

/*
data::json *server::listener::find(string label)
{
	//data::json *result = NULL;
	// after comma, closing bracket, or square bracket
	// add type to temp message queue
	// until we run out of } brackets
	// then pump the whole lot out

	// COMMA IS DIFFERENT -- may represent just a value
	if (current != NULL)
	{
		if (current->identifier().icompare(label)) return current;
	}

	data::json *classes[] = { &message, 
							  &items, 
							  &item, 
							  &lines, 
							  &line, 
							  &components, 
							  &component,
							  &elements,
							  &element };

	for (long i = 0L; i < 9L; ++i)
	{
		if (classes[i]->identifier().icompare(label))
		{
			classes[i]->clear();
			return classes[i];
		}
	}

	return NULL;
}
*/

string server::listener::last()
{
	long i = depth - 1L;
	while (i > 0L)
	{

		string temp(&parents[i][0]);
		if (temp.count() > 0L) return temp;
		--i;
	}

	return string("");
}

string server::listener::FQDN(string label)
{
	string result = label;

	//json *current = _parent;
	long i = depth - 1L;
	while (i > 0L)
	{

		string temp(&parents[i][0]);
		if (temp.count() > 0L)
		{
			if(result.count() > 0) result = temp + "\\" + result;
			else result = temp;
		}
		//if (current->identifier().count() > 0L) result = current->identifier() + "\\" + result;
		//current = _parent->_parent;
		--i;
	};

	return result;
}

// get record, that conforms to template
void server::listener::extract(string parent, string label, string value)
{
	// VALIDATE WE'RE NESTED CORRECTLY
	//if(current->parent.identifier().icompare(find(parent->parent))
	//int moo = 0;
	//if (value.icompare(string("england")))
	//{
		//moo = 2;
	//}
	//Log << "FIND " << last() <<  " [" << FQDN() << "]\r\n";

	//string a = current->FQDN();
	//string b = FQDN();

	//Log << "A[" + a + "] B[" + b + "]\r\n";
	//Log << "FIND " << FQDN() << "\r\n";

	current = task.message.find(FQDN());//last());
	if (current != NULL)
	{
		//Log << "FOUND\r\n";
		//string a = current->FQDN();
		//string b = FQDN();

		//Log << "A[" + a + "] B[" + b + "]\r\n";
		//Log << "[" << current->identifier() << "] [label=" << label << "] value=[" << value << "]\r\n";
		current->add(custom::pair(label, value));
	}
	// parents lines, and components, signify line and component need
	// to be reset
	//components = new data::components::base(line);

//data::components::base moo(line);
	// I should probably validate my labels/characters
	/*
	parent = parent.trim('"');
	parent.toUpper();
	if (parent.compare("MESSAGE") == 0)
	{
		label = label.trim('"');
		label.toUpper();
		if (label.compare("USERID") == 0)
		{
			int userID = value.toInteger();
		}
		else if (label.compare("APIKEY") == 0)
		{
			int apiKey = value.toInteger();
		}

	}
	else if (parent.compare("LINE") == 0)
	{
		label.toUpper();
		if (label.compare("EXCLUSIVITY") == 0)
		{
			//line.exclusivityID = value.toInteger();
		}
	}
	else if (parent.compare("VALUES") == 0)
	{
		// components->component->values
		label.toUpper();
		if (label.compare("VALUE") == 0)
		{
			//component.value = value;
		}
	}
	*/
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

void server::client::reset(manager::manager *manager)
{
	init = false; cleanup();

	this->manager = manager;
		
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

	for (long i = 0; i < s->config.clients; ++i)
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

void server::server::reset(::server::configuration *settings)
{
	init = false; cleanup();
	iterations = 0L; counter = 0L;

	config = *settings;
	//config.copy(settings);

	clients = new client*[config.clients];
	if (clients == NULL) return;

	for (long i = 0L; i < config.clients; ++i)
	{
		clients[i] = new client(config.get());
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
		for (long i = 0L; i < config.clients; ++i)
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
		for (long i = 0L; i < config.clients; ++i)
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
		for (long i = 0L; i < config.clients; ++i)
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

		for (long i = 0L; i < config.clients; ++i)
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

		for (long i = 0L; i < config.clients; ++i)
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

		for (long i = 0L; i < config.clients; ++i)
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
		for (long i = 0L; i < config.clients; ++i)
		{
			if (clients[i] != NULL) delete clients[i];
		}

		delete clients;
	}
}