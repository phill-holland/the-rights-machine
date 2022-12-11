#include "message/message.h"
#include "compute/header.h"

void data::message::message::reset()
{
	init = false; cleanup();

	items.json::parent(this);

	lines.json::parent(&items);
	components.json::parent(&lines);
	elements.json::parent(&components);

	json *identifiers[] = { &elements.temp, &elements,
							&components.temp, &components,
							&lines.temp, &lines,
							&items.temp, &items,
							this };

	for (long i = 0L; i < 9L; ++i)
	{
		hash[identifiers[i]->FQDN()] = identifiers[i];
	}
	
	queue::base *queues[] = { &elements, &components, &lines, &items };
	json *id[] = { &elements, &components, &lines, &items };

	for (long i = 0L; i < 4L; ++i)
	{
		queue_hash[id[i]->FQDN()] = queues[i];
	}

	clear();

	init = true;
}

void data::message::message::clear()
{
	messageID = 0;
	
	guid.clear();
	name.clear();

	created.clear();
	finished.clear();

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

json::request::json *data::message::message::find(string FQDN)
{
	std::unordered_map<string, ::json::request::json*, hasher, equality>::iterator i = hash.find(FQDN);
	if(i != hash.end()) return (::json::request::json*)i->second;

	return NULL;
}

data::message::message data::message::message::split(inquiry &inquiry, mapping &destination)
{
	int in_ptr = 0, out_ptr = 0;

	message result(*this);
	result.lines.clear(false);

	for (long i = 0L; i < lines.count(); ++i)
	{
		models::line::line *source = lines[i];

		for (long j = 0L; j < inquiry.queries.count(); ++j)
		{
			models::query::query *query = inquiry.queries[j];

			if (source->overlapped(*query))
			{
				if (source->typeID == (int)models::line::line::TYPE::in)
				{
					long out_count = 0L;
					for (long k = 0L; k < lines.count(); ++k)
					{
						models::line::line *output = lines[k];
						if (output->typeID == (int)models::line::line::TYPE::out)
						{
							bool splitted = false;
							if(source->overlapped(*output))
							{
								++out_count;
								std::vector<types::zone> splits = source->split(*output);
								for (long l = 0L; l < (long)splits.size(); ++l)
								{
									models::line::line temp = source->spawn(splits[l].start, splits[l].end);
									temp.lineID = source->lineID;

									data::message::mapping::map map;
									map.index = result.lines.count();
									map.lineID = source->lineID;

									result.lines.set(temp);

									destination.in[in_ptr] = map;
									++in_ptr;

									splitted = true;
								}
							}

							if(splitted == false)
							{
								data::message::mapping::map map;
								map.index = result.lines.count();
								map.lineID = source->lineID;
								
								result.lines.set(*source);
								
								destination.in[in_ptr] = map;
								++in_ptr;								
							}
						}
					}

					if(out_count == 0L)
					{
						data::message::mapping::map map;
						map.index = result.lines.count();
						map.lineID = source->lineID;
						
						result.lines.set(*source);
						
						destination.in[in_ptr] = map;
						++in_ptr;
					}
				}
				else if (source->typeID == (int)models::line::line::TYPE::out)
				{
					data::message::mapping::map map;
					map.index = result.lines.count();
					map.lineID = source->lineID;
					
					result.lines.set(*source);				
					destination.out[out_ptr] = map;
					++out_ptr;
				}
			}
		}
	}

	return result;
}
			
void data::message::message::filter(compute::interfaces::row **rows, unsigned long total, 
								    std::unordered_map<int, data::message::mapping::map> &map)
{
	int max_components = components.maximum();
	for (unsigned long i = 0UL; i < total; ++i)
	{
		rows[i]->clear();
	}

	for(std::unordered_map<int, mapping::map>::iterator it = map.begin(); it != map.end(); ++it)
	{
		int index = it->first;
		mapping::map line = it->second;

		for (long h = 0L; h < elements.count(); ++h)
		{
			models::element::element *element = elements[h];
			if(components.mapper::parent(element->componentID) == line.lineID)
			{
				string component = components.name(element->componentID);
				int componentID = components.map(component);

				if(componentID >= 0)
				{
					int itemID = lines.mapper::parent(line.lineID);

					models::line::line *current = lines[line.index];

					unsigned long offset = (index * max_components) + componentID;
					if (offset < total)
					{
						rows[offset]->set(elements.map(element->value));
						
						compute::header temp(current->start, current->end, messageID, itemID, line.lineID, componentID);
						rows[offset]->set(&temp);
					}
				}
			}
		}
	}
}

bool data::message::message::load(file::file<models::item::item> *source)
{
	bool valid = false;

	models::item::item temp;

	while (source->read(temp))
	{
		if (!items.set(temp)) return false;

		valid = true;
	}

	return valid;
}

/*
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
*/
bool data::message::message::load(file::file<models::line::line> *source)
{
	bool valid = false;

	models::line::line temp;

	while (source->read(temp))
	{
		if (!lines.set(temp)) return false;

		valid = true;
	}

	return valid;
}

bool data::message::message::load(file::file<models::component::line::component> *source)
{
	bool valid = false;

	models::component::line::component temp;

	while (source->read(temp))
	{
		if (!components.set(temp)) return false;

		valid = true;
	}

	return valid;
}

bool data::message::message::load(file::file<models::element::element> *source)
{
	bool valid = false;

	models::element::element temp;

	while (source->read(temp))
	{
		if (!elements.set(temp)) return false;

		valid = true;
	}

	return valid;
}

bool data::message::message::save(file::file<models::item::item> *destination)
{
	bool valid = false;

	models::item::item temp;

	for (long i = 0L; i < items.count(); ++i)
	{
		temp = items[i];

		if (!destination->write(temp)) return false;

		valid = true;
	}

	return valid;
}

/*
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
*/

bool data::message::message::save(file::file<models::line::line> *destination)
{
	bool valid = false;

	models::line::line temp;

	for (long i = 0L; i < lines.count(); ++i)
	{
		temp = lines[i];

		if (!destination->write(temp)) return false;

		valid = true;
	}

	return valid;
}

bool data::message::message::save(file::file<models::component::line::component> *destination)
{
	bool valid = false;

	models::component::line::component temp;

	for (long i = 0L; i < components.count(); ++i)
	{
		temp = components[i];

		if (!destination->write(temp)) return false;

		valid = true;
	}

	return valid;
}

bool data::message::message::save(file::file<models::element::element> *destination)
{
	bool valid = false;

	models::element::element temp;

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
	name = source. name;
	guid = source.guid;
	created = source.created;
	finished = source.finished;

	items = source.items;
	lines = source.lines;
	components = source.components;
	elements = source.elements;
}

string data::message::message::output()
{
	string result("\"message\" : {\r\n");
	for (long i = 0L; i < items.count(); ++i) result.concat(items[i]->output());
	for (long i = 0L; i < lines.count(); ++i) result.concat(lines[i]->output());
	for (long i = 0L; i < components.count(); ++i) result.concat(components[i]->output());
	for (long i = 0L; i < elements.count(); ++i) result.concat(elements[i]->output());
	result.concat(string("}\r\n"));

	return result;
}

bool data::message::message::add(core::custom::pair source)
{
	return false;
}

void data::message::message::makeNull()
{
}

void data::message::message::cleanup()
{
}