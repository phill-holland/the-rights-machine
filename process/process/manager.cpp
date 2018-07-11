#include "manager.h"

void manager::manager::reset(long units)
{
	init = false; cleanup();

	total = units;

	read = 0L; write = 0L; out = 0L;

	nodes = new node[total];
	if (nodes == NULL) return;

	init = true;
}

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

void manager::manager::makeNull()
{
	nodes = NULL;
}

void manager::manager::cleanup()
{
	if (nodes != NULL) delete nodes;
}