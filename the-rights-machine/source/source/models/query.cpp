#include "models/query.h"
#include "core/custom/datetime.h"
#include "compute/header.h"

void models::query::query::reset()
{
	init = false; cleanup();

	clear();

	init = true;
}

void models::query::query::clear()
{
	queryID = 0;
	messageID = 0;

	start.clear();
	end.clear();

	components.clear();
	elements.clear();
}

bool models::query::query::add(core::custom::pair source)
{
	if (string("start").icompare(source.name))
	{
		return start.from(source.value);
	}

	if (string("end").icompare(source.name))
	{
		return end.from(source.value);
	}

	return false;
}

void models::query::query::filter(compute::interfaces::row **rows, unsigned long total, unsigned long lines)
{
	int max_components = components.maximum();

	for (unsigned long i = 0UL; i < total; ++i)
	{
		rows[i]->clear();
	}

	for (long h = 0L; h < elements.count(); ++h)
	{
		models::element::element *element = elements[h];
		int queryID = components.mapper::parent(element->componentID);

		if(queryID == this->queryID)
		{
			string component = components.name(element->componentID);

			for (unsigned long i = 0UL; i < lines; ++i)
			{				
				int componentID = components.map(component);
				if(componentID >= 0)
				{
					unsigned long offset = (i * max_components) + ((unsigned long)componentID);//components.map(component);

					if (offset < total)
					{
						string value = elements.name(element->elementID);
						int e = elements.map(value);
						rows[offset]->set(e);

						::compute::header temp(types::datetime(), types::datetime(), messageID, 0, 0, componentID);
						rows[offset]->set(&temp);
					}
				}
			}
		}
	}
}

bool models::query::query::load(file::file<models::component::query::component> *source)
{
	bool valid = false;

	models::component::query::component temp;

	while (source->read(temp))
	{
		if (!components.set(temp)) return false;

		valid = true;
	}

	return valid;
}

bool models::query::query::load(file::file<models::element::element> *source)
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

bool models::query::query::save(file::file<models::component::query::component> *destination)
{
	bool valid = false;

	models::component::query::component temp;

	for (long i = 0L; i < components.count(); ++i)
	{
		if (!destination->write(temp)) return false;

		valid = true;
	}

	return valid;
}

bool models::query::query::save(file::file<models::element::element> *destination)
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

void models::query::query::copy(query const &source)
{
	queryID = source.queryID;
	messageID = source.messageID;
	start = source.start;
	end = source.end;

	components = source.components;
	elements = source.elements;
}

string models::query::query::output()
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

void models::query::query::makeNull()
{
}

void models::query::query::cleanup()
{
}