#include "manager.h"

void manager::manager::reset()
{
	init = false; cleanup();


	read = 0L;

	//nodes = new custom::list<compute::compute*>();
	//if (nodes == NULL) return;
	//if (!nodes->initalised()) return;

	init = true;
}

void manager::manager::add(compute::compute *source)
{
	mutex lock(token);

	nodes.push_back(source);
	//return nodes->add(source);
}

// adds a compute task block
//bool add(node &n);
bool manager::manager::push(compute::task &task)
{
	//return false; 
	long count = 0L;

	mutex lock(token);

	bool result = false;
	do
	{
		if (read >= (long)nodes.size()) read = 0L;
		result = nodes[read++]->push(task);
		++count;
	} while ((!result)&&(count < nodes.size()));

	return result;


	//return (*nodes)[read++]->push(task);
}
/*
bool manager::manager::add(node &n)
{
	mutex lock(token);

	nodes[write++] = n;
	if (write >= total) write = 0L;
}

manager::node *manager::manager::next()
{
	mutex lock(token);

	long count = 0L;
	long i = out;

	while (count < total)
	{
		if (i >= total) i = 0L;
		if (nodes[i].messages != NULL)
		{
			if (!nodes[i].messages->isempty())
			{
				out = i + 1L;
				return &nodes[i];
			}
		}
		++i;
		++count;
	};

	return NULL; // no messages to process
	// for COMPUTE to pull next processing task
}
*/
/*
queue::queue<data::response> *manager::manager::push(data::message::message *m)
{
	mutex lock(token);

	node n = nodes[read++];
	if (read >= total) read = 0L;
	if (n.messages != NULL)
	{
		if (!n.messages->set(*m)) return NULL;
	}

	return n.response;
}
*/
void manager::manager::makeNull()
{
	//nodes = NULL;
}

void manager::manager::cleanup()
{
	//if (nodes != NULL) delete nodes;
}