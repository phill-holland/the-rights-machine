#include "message.h"
#include "header.h"
#include "log.h"

void data::message::message::reset()
{
	init = false; cleanup();
	
	items.json::parent(this);
	queries.json::parent(this);

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
	user = string("");
	apikey = string("");
	guid = string("");

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

void data::message::message::filter(compute::common::row **rows, unsigned long total, std::unordered_map<int, int> &map)
{
	int max_components = components.maximum();
	for (unsigned long i = 0UL; i < total; ++i)
	{
		rows[i]->clear();
	}

	for (long h = 0L; h < elements.count(); ++h)
	{
		data::element::element element = elements[h];
		int lineID = components.mapper::parent(element.componentID);

		std::unordered_map<int, int>::iterator it = map.find(lineID);
		if (it != map.end())
		{
			string component = components.map(element.componentID);
			int componentID = components.map(component);
			int itemID = lines.mapper::parent(lineID);
			unsigned long offset = (map[lineID] * max_components) + componentID;
			if (offset < total)
			{
				rows[offset]->set(elements.map(element.value));
				rows[offset]->set(compute::header(messageID, itemID, lineID, componentID));
			}
		}
	}
}

bool data::message::message::load(file::file<data::item::item> *source)
{
	bool valid = false;

	data::item::item temp;

	while (source->read(temp))
	{
		if (!items.set(temp)) return false;

		valid = true;
	}

	return valid;
}

bool data::message::message::load(file::file<data::query::query> *source)
{
	bool valid = false;

	data::query::query temp;

	while (source->read(temp))
	{
		if (!queries.set(temp)) return false;

		valid = true;
	}

	return valid;
}

bool data::message::message::load(file::file<data::line::line> *source)
{
	bool valid = false;

	data::line::line temp;

	while (source->read(temp))
	{
		if (!lines.set(temp)) return false;

		valid = true;
	}

	return valid;
}

bool data::message::message::load(file::file<data::component::line::component> *source)
{
	bool valid = false;

	data::component::line::component temp;

	while (source->read(temp))
	{
		if (!components.set(temp)) return false;

		valid = true;
	}

	return valid;
}

bool data::message::message::load(file::file<data::element::element> *source)
{
	bool valid = false;

	data::element::element temp;

	while (source->read(temp))
	{
		if (!elements.set(temp)) return false;

		valid = true;
	}

	return valid;
}

bool data::message::message::save(file::file<data::item::item> *destination)
{
	bool valid = false;

	data::item::item temp;

	for (long i = 0L; i < items.count(); ++i)
	{
		temp = items[i];

		if (!destination->write(temp)) return false;

		valid = true;
	}

	return valid;
}

bool data::message::message::save(file::file<data::query::query> *destination)
{
	bool valid = false;

	data::query::query temp;

	for (long i = 0L; i < items.count(); ++i)
	{
		temp = queries[i];

		if (!destination->write(temp)) return false;

		valid = true;
	}

	return valid;
}

bool data::message::message::save(file::file<data::line::line> *destination)
{
	bool valid = false;

	data::line::line temp;

	for (long i = 0L; i < lines.count(); ++i)
	{
		temp = lines[i];

		if (!destination->write(temp)) return false;

		valid = true;
	}

	return valid;
}

bool data::message::message::save(file::file<data::component::line::component> *destination)
{
	bool valid = false;

	data::component::line::component temp;

	for (long i = 0L; i < components.count(); ++i)
	{
		temp = components[i];

		if (!destination->write(temp)) return false;

		valid = true;
	}

	return valid;
}

bool data::message::message::save(file::file<data::element::element> *destination)
{
	bool valid = false;

	data::element::element temp;

	for (long i = 0L; i < elements.count(); ++i)
	{
		temp = elements[i];

		if (!destination->write(temp)) return false;

		valid = true;
	}

	return valid;
}

void data::message::message::copy(message const &source)
{
	messageID = source.messageID;
	user = source.user;
	apikey = source.apikey;
	guid = source.guid;
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
	if (string("user").icompare(source.name))
	{
		user = source.value;
		return true;
	}
	else if (string("apikey").icompare(source.name))
	{
		apikey = source.value;
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