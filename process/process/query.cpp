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

bool data::query::query::add(custom::pair &source)
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

void data::query::query::filter(compute::row **rows, unsigned long total, unsigned long lines)
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
			string component = components.map(element.componentID);

			for (unsigned long i = 0UL; i < lines; ++i)
			{
				unsigned long offset = (i * max_components) + components.map(component);

				if (offset < total)
				{
					rows[offset]->set(elements.map(element.value));
				}
			}
		}
	}
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

void data::query::query::output()
{
	Log << "\"query\" : {\r\n";	
	Log << "\"start\" : \"" << (string)start << "\",\r\n";
	Log << "\"end\" : \"" << (string)end << "\",\r\n";
	for (long i = 0L; i < components.count(); ++i) components[i].output();
	for (long i = 0L; i < elements.count(); ++i) elements[i].output();
	Log << "}\r\n";
}

void data::query::query::makeNull()
{
}

void data::query::query::cleanup()
{
}