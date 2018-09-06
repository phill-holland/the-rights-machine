#include "message.h"
#include "header.h"
#include "log.h"

void data::message::message::reset()
{
	init = false; cleanup();
	
	items.json::parent(this);
	queries.json::parent(this);

	// need to move this into query class???
	queries.temp.components.json::parent(&queries);
	queries.temp.elements.json::parent(&queries.temp.components);

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

	for (long i = 0L; i < 15L; ++i)
	{
		hash[identifiers[i]->FQDN()] = identifiers[i];
	}

	queue::base *queues[] = { &queries, &queries.temp.elements, &queries.temp.components, &elements, &components, &lines, &items };
	json *id[] = { &queries, &queries.temp.elements, &queries.temp.components, &elements, &components, &lines, &items };

	for (long i = 0L; i < 7L; ++i)
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
	GUID = string("");

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

data::json::request::json *data::message::message::find(string FQDN)
{
	std::unordered_map<string, data::json::request::json*, hasher, equality>::iterator i = hash.find(FQDN);
	if(i != hash.end()) return (data::json::request::json*)i->second;

	return NULL;
}

void data::message::message::filter(compute::row **rows, unsigned long total, std::unordered_map<int, int> &map)
{
	Log << "filter 1\r\n";
	int max_components = components.maximum();
	Log << "filter 2\r\n";
	for (unsigned long i = 0UL; i < total; ++i)
	{
		Log << "filter 3\r\n";
		rows[i]->clear();
	}

	Log << "filter 4\r\n";
	for (long h = 0L; h < elements.count(); ++h)
	{
		Log << "filter 5\r\n";
		data::element::element element = elements[h];
		int lineID = components.mapper::parent(element.componentID);
		Log << "filter 6\r\n";
		if (map.find(lineID) != map.end())
		{
			Log << "filter 7\r\n";
			string component = components.map(element.componentID);
			Log << "filter 7.1\r\n";
			int componentID = components.map(component);
			Log << "filter 7.2\r\n";
			int itemID = lines.mapper::parent(lineID);
			Log << "filter 8\r\n";
			unsigned long offset = (map[lineID] * max_components) + componentID;
			if (offset < total)
			{
				Log << "filter 9\r\n";
				(*rows)[offset].set(elements.map(element.value));
				(*rows)[offset].set(compute::header(messageID, itemID, lineID, componentID));
				Log << "filter 10\r\n";
			}
		}
	}
}

void data::message::message::copy(message const &source)
{
	messageID = source.messageID;
	userID = source.userID;
	GUID = source.GUID;
	created = source.created;
	finished = source.finished;

	queries = source.queries;
	items = source.items;
	lines = source.lines;
	components = source.components;
	elements = source.elements;	
}

void data::message::message::output()
{
	Log << "\"message\" : {\r\n";
	for (long i = 0L; i < queries.count(); ++i) queries[i].output();
	for (long i = 0L; i < items.count(); ++i) items[i].output();
	for (long i = 0L; i < lines.count(); ++i) lines[i].output();
	for (long i = 0L; i < components.count(); ++i) components[i].output();
	for (long i = 0L; i < elements.count(); ++i) elements[i].output();
	Log << "}\r\n";
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