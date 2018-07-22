#include "cpu.h"
#include "string.h"
#include <unordered_map>
#include <unordered_set>
#include <vector>

void compute::cpu::processor::reset(unsigned long width, unsigned long height)
{
	init = false; cleanup();

	this->width = width; this->height = height;

	in = new grid(width, height);
	if (in == NULL) return;
	if (!in->initalised()) return;

	out = new grid(width, height);
	if (out == NULL) return;
	if (!out->initalised()) return;

	rows = new row*[height];
	if (rows == NULL) return;
	for (unsigned long i = 0UL; i < height; ++i)
	{
		rows[i] = new row(width);
		if (rows[i] == NULL) return;
		if (!rows[i]->initalised()) return;
	}

	inputs = new data::line::line[width];
	if (inputs == NULL) return;

	outputs = new data::line::line[width];
	if (outputs == NULL) return;

	input_ptr = output_ptr = 0UL;

	init = true;
}

void compute::cpu::processor::clear()
{
	input_ptr = output_ptr = 0UL;

	in->clear();
	out->clear();

	for (unsigned long i = 0UL; i < width; ++i)
	{
		inputs[i].clear();
		outputs[i].clear();
	}
	
	for (unsigned long i = 0UL; i < height; ++i)
	{
		rows[i]->clear();
	}	
}

void compute::cpu::processor::push(data::message::message &message)
{
	clear();

	std::unordered_map<int, int> in_map, out_map;
	int in_ptr = 0, out_ptr = 0;

	for (long i = 0L; i < message.lines.count(); ++i)
	{
		data::line::line source = message.lines[i];

		for (long j = 0L; j < message.queries.count(); ++j)
		{
			data::query::query query = message.queries[j];

			if (source.overlapped(query))
			{
				if (source.typeID == (int)data::line::line::TYPE::in)
				{
					in_map[source.lineID] = in_ptr++;
					for (long k = 0L; k < message.lines.count(); ++k)
					{
						data::line::line output = message.lines[k];
						if (output.typeID == (int)data::line::line::TYPE::out)
						{
							std::vector<zone::zone> result = source.split(output);
							for (long l = 0L; l < result.size(); ++l)
							{
								inputs[input_ptr++] = source.spawn(result[l].start, result[l].end);
							}
						}
					}
				}
				else if (source.typeID == (int)data::line::line::TYPE::out)
				{
					out_map[source.lineID] = out_ptr++;
					outputs[output_ptr++].copy(source);
				}
			}
		}
	}
	
	// check outputs overlap input dates

	if (input_ptr > 0UL)
	{
		filter(message, rows, in_map);

		for (unsigned long i = 0UL; i < (in_map.size() * message.components.maximum()); ++i)
		{
			in->push(*rows[i]);
		}

		if (output_ptr > 0L)
		{
			filter(message, rows, out_map);
			
			unsigned long offset = 0UL;

			for (unsigned long k = 0UL; k < out_map.size(); ++k)
			{
				out->clear();
				for (unsigned long j = 0UL; j < in_map.size(); ++j)
				{					
					for (unsigned long i = 0UL; i < (unsigned long)message.components.maximum(); ++i)
					{
						out->push(*rows[offset + i]);
					}
				}

				// add datetime start/end to header
				// and check overlaps during minus
				// or push?

				in->minus(*out);
				offset += message.components.maximum();
			}
		}
	}
}

void compute::cpu::processor::filter(data::message::message &message, row **rows, std::unordered_map<int, int> &map)
{
	int max_components = message.components.maximum();

	for (unsigned long i = 0UL; i < height; ++i)
	{
		rows[i]->clear();
	}

	for (long h = 0L; h < message.elements.count(); ++h)
	{
		data::element::element element = message.elements[h];
		int lineID = message.components.mapper::parent(element.componentID);

		if (map.find(lineID) != map.end())
		{
			string component = message.components.map(element.componentID);
			int itemID = message.lines.mapper::parent(lineID);

			unsigned long offset = (map[lineID] * max_components) + message.components.map(component);
			if (offset < height)
			{
				(*rows)[offset].set(message.elements.map(element.value));
				(*rows)[offset].set(header(message.messageID, itemID, lineID));
			}
		}
	}
}

void compute::cpu::processor::makeNull()
{
	in = NULL;
	out = NULL;
	rows = NULL;
	inputs = NULL;
	outputs = NULL;
}

void compute::cpu::processor::cleanup()
{
	if (outputs != NULL) delete outputs;
	if (inputs != NULL) delete inputs;
	if (rows != NULL)
	{
		for (unsigned long i = height - 1UL; i >= 0UL; i--)
		{
			if (rows[i] != NULL) delete rows[i];
		}
		delete rows;
	}
	if (out != NULL) delete out;
	if (in != NULL) delete in;
}

void compute::cpu::cpu::process()
{

}

DWORD WINAPI compute::cpu::cpu::background(thread *bt)
{
	return (DWORD)0;
}

void compute::cpu::cpu::reset(::queue::factory<::compute::task> *factory)
{
	init = false; cleanup();

	queue = factory->get();
	if (queue == NULL) return;

	init = true;
}

void compute::cpu::cpu::makeNull()
{
}

void compute::cpu::cpu::cleanup()
{
}

// has many worker threads for grid subtraction

// need to add query section in JSON source

// json validate
// component without name, error
// validate number of components
// validate max elements per component
// validate JSON string to integer conversions

/*
discount lines where
line.dateend < selection.datestart

discount lines where
line.datestart > line.dateend
and line.datestart == line.dateend

index = 0
for each component
	hashmap[string] = index
	hashmap[index] = string
	++index

// split into descrete blocks
// by date only	
for all in
	for all out
		if overlap
			inners.add(split(in - out))
		else inners.add(in)

index = 0		
foreach output
	in_array.start = inner.start
	in_array.end = inner.end
	in_array.itemID = inner.itemID
	in_array.messageID = inner.messageID
	foreach component
		in_array[index++][hashmap[string]] = 1
		
index = 0
foreach out
	foreach component
		out_array[index++][hashmap[string]] = 1

result = in_array - out_array 
result = in_array & selection_array

// loop through each block
// compare components
// if all valid for selection
// combine av date ranges

if (result == selection_array) ok;
*/