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
	data::message::message msg = task.message.split(task.inquiry, mappings);

	if(mappings.hasIn())
	{
		msg.filter(rows, height, mappings.in);
		for (unsigned long i = 0UL; i < (mappings.in.size() * msg.components.maximum()); ++i)
		{
			in->push(rows[i]);
		}

		if(mappings.hasOut())
		{
			msg.filter(rows, height, mappings.out);

			unsigned long offset = 0UL;

			for (unsigned long k = 0UL; k < (unsigned long)mappings.out.size(); ++k)
			{
				out->clear();
				for (unsigned long j = 0UL; j < (unsigned long)mappings.in.size(); ++j)
				{
					for (unsigned long i = 0UL; i < (unsigned long)msg.components.maximum(); ++i)
					{
						out->push(rows[offset + i]);
					}
				}

				in->minus(*out);

				offset += msg.components.maximum();
			}
		}

		unsigned long offset = 0UL;
		for (unsigned long i = 0UL; i < (unsigned long)task.inquiry.queries.count(); ++i)
		{
			data::query::query *q = task.inquiry.queries[i];
			q->filter(rows, height, (unsigned long)mappings.in.size());

			query->clear();
			for (unsigned long j = 0UL; j < (unsigned long)mappings.in.size(); ++j)
			{
				for (unsigned long k = 0UL; k < (unsigned long)msg.components.maximum(); ++k)
				{
					query->push(rows[offset + k]);
				}
			}

			in->AND(*query);
			bool result = in->compare(*query);

			if(task.response != NULL)
			{
				data::response::response response;

				response.guid = msg.guid;
				response.name = msg.name;
				response.available = result;
				response.created = datetime::now();

				task.response->set(response);
			}

			if(task.notify != NULL) task.notify->notifyOut(msg.guid);
			offset += msg.components.maximum();
		}
	}
	else
	{
		if(task.response != NULL)
		{
			data::response::response response;

			response.guid = msg.guid;
			response.name = msg.name;
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
	::compute::task task;
	if (get(task))
	{
		process->push(task);
	}
	else
	{
		sleep(250);
	}

	sleep(100);
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
