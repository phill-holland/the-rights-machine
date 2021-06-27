#include "query.h"

void data::query::query::reset()
{
	init = false; cleanup();

	clear();

	init = true;
}

void data::query::query::clear()
{
	queryID = 0;
	messageID = 0;

	start.clear();
	end.clear();

	components.clear();
	elements.clear();
}

bool data::query::query::add(custom::pair source)
{
	if (string("start").icompare(source.name))
	{
		start.from(source.value);
		return true;
	}

	if (string("end").icompare(source.name))
	{
		end.from(source.value);
		return true;
	}

	return false;
}

void data::query::query::filter(compute::common::row **rows, unsigned long total, unsigned long lines)
{
	int max_components = components.maximum();

	for (unsigned long i = 0UL; i < total; ++i)
	{
		rows[i]->clear();
	}

	for (long h = 0L; h < elements.count(); ++h)
	{
		data::element::element element = elements[h];
		int queryID = components.mapper::parent(element.componentID);

		if(queryID == this->queryID)
		{
			string component = components.name(element.componentID);

			for (unsigned long i = 0UL; i < lines; ++i)
			{
				int componentID = components.map(component);
				if(componentID >= 0)
				{
					unsigned long offset = (i * max_components) + ((unsigned long)componentID);//components.map(component);

					if (offset < total)
					{
						string value = elements.name(element.elementID);
						int e = elements.map(value);//element.value);
						rows[offset]->set(e);
						//compute::header temp(messageID, itemID, lineID, componentID);
						compute::header temp(messageID, 0, 0, componentID);
						rows[offset]->set(temp);
					}
				}
			}
		}
	}
}

bool data::query::query::load(file::file<data::component::query::component> *source)
{
	bool valid = false;

	data::component::query::component temp;

	while (source->read(temp))
	{
		if (!components.set(temp)) return false;

		valid = true;
	}

	return valid;
}

bool data::query::query::load(file::file<data::element::element> *source)
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

bool data::query::query::save(file::file<data::component::query::component> *destination)
{
	bool valid = false;

	data::component::query::component temp;

	for (long i = 0L; i < components.count(); ++i)
	{
		if (!destination->write(temp)) return false;

		valid = true;
	}

	return valid;
}

bool data::query::query::save(file::file<data::element::element> *destination)
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

void data::query::query::copy(query const &source)
{
	queryID = source.queryID;
	messageID = source.messageID;
	start = source.start;
	end = source.end;

	components = source.components;
	elements = source.elements;
}

string data::query::query::output()
{
	string result("\"query\" : {\r\n");
	result.concat(string("\"start\" : \""));
	result.concat((string)start); //<< "\",\r\n";
	result.concat(string("\",\r\n\"end\" : \""));
	result.concat((string)end);
	result.concat(string("\",\r\n"));
	for (long i = 0L; i < components.count(); ++i) result.concat(components[i]->output());
	for (long i = 0L; i < elements.count(); ++i) result.concat(elements[i]->output());
	result.concat(string("}\r\n"));

	return result;
}

void data::query::query::makeNull()
{
}

void data::query::query::cleanup()
{
}