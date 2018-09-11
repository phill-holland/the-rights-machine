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

	query = new grid(width, height);
	if (query == NULL) return;
	if (!query->initalised()) return;

	Log << "WIDTH,HEIGHT " << width << " " << height << "\r\n";
	rows = new row*[height];
	if (rows == NULL) return;
	for (unsigned long i = 0UL; i < height; ++i) rows[i] = NULL;

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

//void compute::cpu::processor::push(data::message::message &message)
void compute::cpu::processor::push(::compute::task &task)
{
	clear();
	Log << "ACTUAL MOTHERING FUCKING PUSH\r\n";

	std::unordered_map<int, int> in_map, out_map;
	int in_ptr = 0, out_ptr = 0;

	for (long i = 0L; i < task.message.lines.count(); ++i)
	{
		Log << "meow1\r\n";
		data::line::line source = task.message.lines[i];
		Log << "query count " << task.message.queries.count() << "\r\n";

		// ***
		// if no query generate ERROR, somewhere
		// ***

		for (long j = 0L; j < task.message.queries.count(); ++j)
		{
			Log << "meow2\r\n";
			data::query::query query = task.message.queries[j];

			if (source.overlapped(query))
			{
				Log << "meow3\r\n";
				if (source.typeID == (int)data::line::line::TYPE::in)
				{
					Log << "meow4\r\n";
					//bool split = false;
					in_map[source.lineID] = in_ptr++;
					for (long k = 0L; k < task.message.lines.count(); ++k)
					{
						Log << "meow5\r\n";
						data::line::line output = task.message.lines[k];
						if (output.typeID == (int)data::line::line::TYPE::out)
						{
							Log << "meow6\r\n";
							std::vector<zone::zone> result = source.split(output);
							for (long l = 0L; l < result.size(); ++l)
							{
								inputs[input_ptr++] = source.spawn(result[l].start, result[l].end);
								//split = true;
								Log << "woof-1\r\n";
							}
						}
					}

					//if (!split)
					//{
						//Log << "no split found\r\n";
						//inputs[input_ptr++] = source;
					//}
				}
				else if (source.typeID == (int)data::line::line::TYPE::out)
				{
					Log << "woof-2\r\n";
					out_map[source.lineID] = out_ptr++;
					outputs[output_ptr++].copy(source);
				}
			}
		}
	}
	
	// check outputs overlap input dates

	// push one acquired line, one excluded line and one query line
	// at one time

	// need to decode grid

	Log << "woof1\r\n";
	//if (input_ptr > 0UL)
	if(in_ptr > 0)
	{
		Log << "woof2\r\n";
		task.message.filter(rows, height, in_map);
		Log << "woof2.1\r\n";
		for (unsigned long i = 0UL; i < (in_map.size() * task.message.components.maximum()); ++i)
		{
			Log << "woof2.2\r\n";
			in->push(*rows[i]);
			Log << "woof2.3\r\n";
		}

		//if (output_ptr > 0L)
		if(out_ptr > 0)
		{
			Log << "woof3\r\n";
			task.message.filter(rows, height, out_map);
			
			unsigned long offset = 0UL;

			for (unsigned long k = 0UL; k < (unsigned long)out_map.size(); ++k)
			{
				out->clear();
				for (unsigned long j = 0UL; j < (unsigned long)in_map.size(); ++j)
				{					
					for (unsigned long i = 0UL; i < (unsigned long)task.message.components.maximum(); ++i)
					{
						out->push(*rows[offset + i]);
					}
				}

				// add datetime start/end to header
				// and check overlaps during minus
				// or push?

				in->minus(*out);
				offset += task.message.components.maximum();
			}
		}

		Log << "woof14r\n";
		unsigned long offset = 0UL;
		for (unsigned long i = 0UL; i < (unsigned long)task.message.queries.count(); ++i)
		{
			data::query::query q = task.message.queries[i];

			q.filter(rows, height, (unsigned long)in_map.size());

			query->clear();
			for (unsigned long j = 0UL; j < (unsigned long)in_map.size(); ++j)
			{
				for (unsigned long k = 0UL; k < (unsigned long)task.message.components.maximum(); ++k)
				{
					query->push(*rows[offset + k]);
				}
			}

			Log << "in\r\n";
			//in->output();
			
			Log << "\r\nquery\r\n";
			//query->output();

			in->and(*query);
			Log << "and\r\n";
			bool result = in->compare(*query);
			Log << "compare\r\n";
			data::response::response response;
			response.queryID = q.queryID;
			response.GUID = task.message.GUID;
			response.userID = task.message.userID;
			response.available = result;
			response.created = datetime::now();
			//response.status = string("OK");
			Log << "set response\r\n";
			task.response->set(response);
			Log << "setted\r\n";
			// need to validate query components is same as message.components.maximum()
			offset += task.message.components.maximum();
			Log << "inc offset\r\n";
		}
	}
	else
	{
		// no acquired rights, throw error, not available here!!!
	}
	Log << "PROCESS CPU DONE\r\n";
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

DWORD WINAPI compute::cpu::cpu::background(thread *bt)
{
	Sleep(100);

	Log << "TICK\r\n";
	::compute::task task;
	if (get(task))
	{
		Log << "CPU GET\r\n";
		process->push(task);
	}
	else
	{
		Log << "CPU SLEEP\r\n";
		Sleep(5000);
	}

	return (DWORD)0;
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