#include "task.h"


void compute::task::reset()
{
	init = false; cleanup();

	message.items.json::parent(this);
	inquiry.queries.json::parent(this);

	inquiry.queries.temp.components.json::parent(&inquiry.queries);
	inquiry.queries.temp.elements.json::parent(&inquiry.queries.temp.components);

	message.lines.json::parent(&message.items);
	message.components.json::parent(&message.lines);
	message.elements.json::parent(&message.components);

	json *identifiers[] = { &message.elements.temp, &message.elements,
							&message.components.temp, &message.components,
							&message.lines.temp, &message.lines,
							&message.items.temp, &message.items,
							&inquiry.queries.temp.elements.temp, &inquiry.queries.temp.elements,
							&inquiry.queries.temp.components.temp, &inquiry.queries.temp.components,
							&inquiry.queries.temp, &inquiry.queries, 
							this };

	for (long i = 0L; i < 15L; ++i)
	{
		hash[identifiers[i]->FQDN()] = identifiers[i];
	}
	
	queue::base *queues[] = { &inquiry.queries, &inquiry.queries.temp.elements, &inquiry.queries.temp.components, &message.elements, &message.components, &message.lines, &message.items };
	json *id[] = { &inquiry.queries, &inquiry.queries.temp.elements, &inquiry.queries.temp.components, &message.elements, &message.components, &message.lines, &message.items };

	for (long i = 0L; i < 7L; ++i)
	{
		queue_hash[id[i]->FQDN()] = queues[i];
	}

	clear();

	init = true;
}

void compute::task::clear()
{
    inquiry.clear();
    message.clear();
}

bool compute::task::add(custom::pair source)
{
    return false;
}

queue::base *compute::task::findQ(string FQDN)
{
    std::unordered_map<string, queue::base*, hasher, equality>::iterator i = queue_hash.find(FQDN);
	if (i != queue_hash.end()) return (queue::base*)i->second;

	return NULL;
}

data::json::request::json *compute::task::find(string FQDN)
{
    std::unordered_map<string, data::json::request::json*, hasher, equality>::iterator i = hash.find(FQDN);
	if(i != hash.end()) return (data::json::request::json*)i->second;

    return NULL;
}
