#include "message.h"
#include "log.h"

void data::message::message::reset()
{
	init = false; cleanup();
	
	items.json::parent(this);
	queries.json::parent(this);
	lines.json::parent(&items);
	components.json::parent(&lines);
	elements.json::parent(&components);
	
	json *identifiers[] = { &elements.temp, &elements, 
							&components.temp, &components, 
							&lines.temp, &lines, 
							&items.temp, &items, 
							&queries.temp.elements.temp, &queries.temp.elements,
							&queries.temp.components.temp, &queries.temp.components,
							&queries.temp, &queries, this };

	for (long i = 0L; i < 11L; ++i)
	{
		Log << "ME " << identifiers[i]->identifier() << "\r\n";
		Log << "FQDN " << identifiers[i]->FQDN() << "\r\n";

		hash[identifiers[i]->FQDN()] = identifiers[i];
	}

	queue::base *queues[] = { &queries, &queries.temp.elements, &queries.temp.components, &elements, &components, &lines, &items };
	json *id[] = { &queries, &queries.temp.elements, &queries.temp.components, &elements, &components, &lines, &items };

	for (long i = 0L; i < 5L; ++i)
	{
		queue_hash[id[i]->FQDN()] = queues[i];
	}

	clear();

	init = true;
}

void data::message::message::clear()
{
	messageID = 0;
	userID = 0;
	created.clear();
	finished.clear();

	queries.clear();
	items.clear();
	lines.clear();
	components.clear();
	elements.clear();
}

queue::base *data::message::message::findQ(string FQDN)
{
	std::unordered_map<string, queue::base*, hasher, equality>::iterator i = queue_hash.find(FQDN);
	if (i != queue_hash.end()) return (queue::base*)i->second;

	return NULL;
}

data::json *data::message::message::find(string FQDN)
{
	std::unordered_map<string, data::json*, hasher, equality>::iterator i = hash.find(FQDN);
	if(i != hash.end()) return (data::json*)i->second;

	return NULL;
}

void data::message::message::copy(message const &source)
{
	messageID = source.messageID;
	userID = source.userID;
	created = source.created;
	finished = source.finished;

	items = source.items;
	lines = source.lines;
	components = source.components;
	elements = source.elements;	
}

bool data::message::message::add(custom::pair &source)
{
	if (string("userID").icompare(source.name))
	{
		userID = source.value.toInteger();
		return true;
	}

	return false;
}

void data::message::message::makeNull()
{
}

void data::message::message::cleanup()
{
}