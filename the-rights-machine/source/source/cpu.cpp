#include "cpu.h"
#include "custom/string.h"
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

	query = new grid(width, height);
	if (query == NULL) return;
	if (!query->initalised()) return;

	rows = new ::compute::common::row*[height];
	if (rows == NULL) return;
	for (unsigned long i = 0UL; i < height; ++i) rows[i] = NULL;

	for (unsigned long i = 0UL; i < height; ++i)
	{
		::compute::cpu::row *temp = new ::compute::cpu::row(width);
		if (temp == NULL) return;
		if (!temp->initalised()) return;

		rows[i] = temp;
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
	query->clear();

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

void compute::cpu::processor::push(::compute::task &task)
{
	clear();

	data::message::mapping mappings;
	data::message::message msg = task.message.split(mappings);

	//std::cout << task.message.output();
	//std::cout << msg.output();

	//if(in_ptr > 0)
	if(mappings.hasIn())
	{
		//task.message.filter(rows, height, in_map);
		msg.filter(rows, height, mappings.in);
		for (unsigned long i = 0UL; i < (mappings.in.size() * msg.components.maximum()); ++i)
		{
			in->push(rows[i]);
		}

		//if(out_ptr > 0)
		if(mappings.hasOut())
		{
			// filter wrong??
			//task.message.filter(rows, height, out_map);
			msg.filter(rows, height, mappings.out);

			unsigned long offset = 0UL;

			for (unsigned long k = 0UL; k < (unsigned long)mappings.out.size(); ++k)
			{
				out->clear();
				for (unsigned long j = 0UL; j < (unsigned long)mappings.in.size(); ++j)
				{
					for (unsigned long i = 0UL; i < (unsigned long)msg.components.maximum(); ++i)
					{
						// rows have invalid header
						out->push(rows[offset + i]);

						//std::cout << "outfgfgf\n";
				//std::cout << out->output();
					}
				}

				// add datetime start/end to header
				// and check overlaps during minus
				// or push?

				//std::cout << "in\n";

				//std::cout << in->output();
				//std::cout << "out\n";
				//std::cout << out->output();

				in->minus(*out);
				//std::cout << "in - out\n";
				//std::cout << in->output();

				offset += msg.components.maximum();
			}
		}

		unsigned long offset = 0UL;
		for (unsigned long i = 0UL; i < (unsigned long)msg.queries.count(); ++i)
		{
			data::query::query *q = msg.queries[i];

			q->filter(rows, height, (unsigned long)mappings.in.size());

			query->clear();
			for (unsigned long j = 0UL; j < (unsigned long)mappings.in.size(); ++j)
			{
				for (unsigned long k = 0UL; k < (unsigned long)msg.components.maximum(); ++k)
				{
					query->push(rows[offset + k]);
				}
			}

			//std::cout << "query\n";
			//std::cout << query->output();

			in->AND(*query);
			bool result = in->compare(*query);

			if(task.response != NULL)
			{
				data::response::response response;

				//response.queryID = q.queryID;
				response.guid = msg.guid;
				//response.name = task.message.items[0].name;
				//response.user = task.message.user;
				response.available = result;
				response.created = datetime::now();

				task.response->set(response);
			}

			if(task.notify != NULL) task.notify->notifyOut(msg.guid);
			// need to validate query components is same as message.components.maximum()
			offset += msg.components.maximum();
		}
	}
	else
	{
		//std::cout << "nothing!\n";
		// no acquired rights, throw error, not available here!!!

		if(task.response != NULL)
		{
			data::response::response response;

			response.guid = msg.guid;
			response.status = data::response::response::STATUS::RANGE;
			response.available = false;
			response.created = datetime::now();

			task.response->set(response);
		}

		if(task.notify != NULL) task.notify->notifyOut(msg.guid);
	}
}

void compute::cpu::processor::makeNull()
{
	in = NULL;
	out = NULL;
	query = NULL;
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
		for (long i = (height - 1L); i >= 0L; i--)
		{
			if (rows[i] != NULL) delete rows[i];
		}

		delete rows;
	}
	if (query != NULL) delete query;
	if (out != NULL) delete out;
	if (in != NULL) delete in;
}

void compute::cpu::cpu::background(thread *bt)
{
	sleep(100);

	::compute::task task;
	if (get(task))
	{
		process->push(task);
	}
	else
	{
		sleep(500);
	}
}

void compute::cpu::cpu::reset(::queue::factory<::compute::task> *factory)
{
	init = false; cleanup();

	queue = factory->get();
	if (queue == NULL) return;

	process = new processor(255, 255);
	if (process == NULL) return;
	if (!process->initalised()) return;

	init = true;
}

void compute::cpu::cpu::makeNull()
{
	process = NULL;
}

void compute::cpu::cpu::cleanup()
{
	if (process != NULL) delete process;
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