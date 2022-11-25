#include "gpu.h"
#include "core/string/string.h"
#include <unordered_map>
#include <unordered_set>
#include <vector>

void compute::gpu::processor::reset(unsigned long width, unsigned long height)
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
		::compute::gpu::row *temp = new ::compute::gpu::row(width);
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

void compute::gpu::processor::clear()
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

void compute::gpu::processor::push(::compute::task &task)
{
	clear();

	std::unordered_map<int, int> in_map, out_map;
	int in_ptr = 0, out_ptr = 0;

	for (long i = 0L; i < task.message.lines.count(); ++i)
	{
		data::line::line source = task.message.lines[i];

		// ***
		// if no query generate ERROR, somewhere
		// ***

		for (long j = 0L; j < task.inquiry.queries.count(); ++j)
		{
			data::query::query query = task.inquiry.queries[j];

			if (source.overlapped(query))
			{
				if (source.typeID == (int)data::line::line::TYPE::in)
				{
					in_map[source.lineID] = in_ptr++;
					for (long k = 0L; k < task.message.lines.count(); ++k)
					{
						data::line::line output = task.message.lines[k];
						if (output.typeID == (int)data::line::line::TYPE::out)
						{
							std::vector<zone::zone> result = source.split(output);
							for (long l = 0L; l < (long)result.size(); ++l)
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

	// push one acquired line, one excluded line and one query line
	// at one time

	// need to decode grid

	if (in_ptr > 0)
	{
		//task.message.filter(rows, height, in_map);
		for (unsigned long i = 0UL; i < (in_map.size() * task.message.components.maximum()); ++i)
		{
			in->push(rows[i]);
		}

		if (out_ptr > 0)
		{
			//task.message.filter(rows, height, out_map);

			unsigned long offset = 0UL;

			for (unsigned long k = 0UL; k < (unsigned long)out_map.size(); ++k)
			{
				out->clear();
				for (unsigned long j = 0UL; j < (unsigned long)in_map.size(); ++j)
				{
					for (unsigned long i = 0UL; i < (unsigned long)task.message.components.maximum(); ++i)
					{
						out->push(rows[offset + i]);
					}
				}

				// add datetime start/end to header
				// and check overlaps during minus
				// or push?

				in->minus(*out);
				offset += task.message.components.maximum();
			}
		}

		unsigned long offset = 0UL;
		for (unsigned long i = 0UL; i < (unsigned long)task.inquiry.queries.count(); ++i)
		{
			data::query::query q = task.inquiry.queries[i];

			q.filter(rows, height, (unsigned long)in_map.size());

			query->clear();
			for (unsigned long j = 0UL; j < (unsigned long)in_map.size(); ++j)
			{
				for (unsigned long k = 0UL; k < (unsigned long)task.message.components.maximum(); ++k)
				{
					query->push(rows[offset + k]);
				}
			}

			in->AND(*query);
			bool result = in->compare(*query);

			data::response::response response;
			//response.queryID = q.queryID;
			response.guid = task.message.guid;
			//response.user = task.message.user;
			response.available = result;
			response.created = datetime::now();

			task.response->set(response);

			// need to validate query components is same as message.components.maximum()
			offset += task.message.components.maximum();
		}
	}
	else
	{
		// no acquired rights, throw error, not available here!!!
	}
}

void compute::gpu::processor::makeNull()
{
	in = NULL;
	out = NULL;
	query = NULL;
	rows = NULL;
	inputs = NULL;
	outputs = NULL;
}

void compute::gpu::processor::cleanup()
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

void compute::gpu::gpu::background(thread *bt)
{
	sleep(100);

	::compute::task task;
	if (get(task))
	{
		process->push(task);
	}
	else
	{
		sleep(5000);
	}

	//return (DWORD)0;
}

void compute::gpu::gpu::reset(::queue::factory<::compute::task> *factory)
{
	init = false; cleanup();

	queue = factory->get();
	if (queue == NULL) return;

	process = new processor(255, 255);
	if (process == NULL) return;
	if (!process->initalised()) return;

	init = true;
}

void compute::gpu::gpu::makeNull()
{
	process = NULL;
}

void compute::gpu::gpu::cleanup()
{
	if (process != NULL) delete process;
}
