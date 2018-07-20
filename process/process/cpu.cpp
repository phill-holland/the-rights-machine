#include "cpu.h"
#include "string.h"
#include <unordered_map>
#include <vector>

void compute::cpu::processor::reset(unsigned long width, unsigned long height)
{
	init = false; cleanup();

	this->width = width; this->height = height;

	in = new grid(width, height);
	if (in == NULL) return;
	if (!in->initalised()) return;

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

	for (unsigned long i = 0L; i < width; ++i)
	{
		inputs[i].clear();
		outputs[i].clear();
	}

}

void compute::cpu::processor::push(data::message::message &message)
{
	clear();

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
					for (long k = 0L; k < message.lines.count(); ++k)
					{
						data::line::line output = message.lines[k];
						if (output.typeID == (int)data::line::line::TYPE::out)
						{
							std::vector<zone::zone> result = source.split(output);
							for (long l = 0L; l < result.size; ++l)
							{
								inputs[input_ptr++] = source.spawn(result[l].start, result[l].end);
							}
						}
					}
				}
				else if (source.typeID == (int)data::line::line::TYPE::out)
				{
					outputs[output_ptr++].copy(source);
				}
			}
		}
	}

	if (input_ptr > 0L)
	{
		grid binputs[15];  //set component horizontally

		// per element, also need to map by parent / component

		// item, lineID
		for (long i = 0L; i < message.elements.count(); ++i)
		{
			data::element::element temp = message.elements[i];
			long y = 0L; // temp.lineID
			//if(temp.lineID in inputs) then OK

			// ****
			// lookup lineID in inputs and get position!!
			// hmmm, not quite
			// ****
			// need another mapping, from compoentID to name to index
			// elementID needs to be normalised - each different component to start from zero
			int a = message.components.map(message.components.identity(temp.componentID));
			int b = message.elements.map(temp.value);
			binputs[a].set(b, y);
			// if lineID in inputs
			//temp.componentID
		}

		//int x_idx = item_idx + 
		// calculate in-lines per item
		// calculate out-lines per item
		//items->lines->components->elements
		//for (long i = 0L; i < message.items.count(); ++i)
		//{
		//}

		// loop through all elements, add to hash map
		// check element is in inputs[]

		// in allocators, for flush function
		// add name, index to hashmap

		// public: int map(string name);
		// protected: void map(string name, int key); ..??
	}

	if(output_ptr > 0L)
	{
	}
}

void compute::cpu::processor::makeNull()
{
	in = NULL;
	inputs = NULL;
	outputs = NULL;
}

void compute::cpu::processor::cleanup()
{
	if (outputs != NULL) delete outputs;
	if (inputs != NULL) delete inputs;
	if (in != NULL) delete in;
}

void compute::cpu::cpu::process()
{
	::compute::task task;
	if (this->get(task))
	{
		// check overlaps
		// with dates here (source queries)

		// componentID on the input, needs to be incremental and not reset
		// for each new line/item

		// ****
		// do hash mappings whilst
		// JSON is receiving
		// ****

		// Queue get loop no good for this

		// ADD CURSOR TO ALLOCATOR

		data::line::line lines[255];
		int ln_out_ptr = 0;
		//data::line::line ln_in;
		//while (task.message.lines.get(ln_in))
		for (long i = 0L; i< task.message.lines.count(); ++i)
		{
			data::line::line ln_in = task.message.lines[i];

			// check query dates
			// ***
			bool invalid = false;

			for (long k = 0; k < task.message.queries.count(); ++k)
			{
				data::query::query q = task.message.queries[k];

				// note I removed the ! at the beginning
				if ((q.start > ln_in.end) || (ln_in.start > q.end))
				{
					invalid = true;
				}
			}
			// ***

			if (!invalid)
			{
				if (ln_in.typeID == 0)
				{
					data::line::line ln_out;
					bool overlap = false;
					//while (task.message.lines.get(ln_out))
					for (long j = 0L; j < task.message.lines.count(); ++j) // hmmmm, cursor will fuck up????
					{
						if (j != i)
						{
							data::line::line ln_out = task.message.lines[j];
							if (ln_out.typeID >= 1)
							{
								if (!((ln_in.start > ln_out.end) || (ln_out.start > ln_in.end)))
								{
									if ((ln_in.start != ln_out.start) && (ln_in.end != ln_out.end))
									{
										datetime a = ln_in.start;
										datetime b = ln_in.end;
										datetime c = ln_out.start;
										datetime d = ln_out.end;

										// ****
										//sort(a, b, c, d);
										// ****

										lines[ln_out_ptr].start = a;
										lines[ln_out_ptr].end = b;
										lines[ln_out_ptr].lineID = ln_in.lineID;
										++ln_out_ptr;

										lines[ln_out_ptr].start = b;
										lines[ln_out_ptr].end = c;
										lines[ln_out_ptr].lineID = ln_in.lineID;
										++ln_out_ptr;

										lines[ln_out_ptr].start = c;
										lines[ln_out_ptr].end = d;
										lines[ln_out_ptr].lineID = ln_in.lineID;
										++ln_out_ptr;

										overlap = true;
									}
								}
							}
						}
					};

					if (!overlap)
					{
						lines[ln_out_ptr++] = ln_in;
					}
				}
				else if (ln_in.typeID == 1)
				{
					// add out lines to special array

					// also need to populate query into block too
					// for comparison

					// if out block don't match query dates
					// fuck it

					// if out block doesn't overlap any in blocks
					// fuck it too
				}
			}
		};

		// USE LINES ARRAY INSTEAD 
		// still need to consider how to map components to new lines

		std::unordered_map<int, int> lineToType;
		std::unordered_map<int, int> componentToLine;

		std::unordered_map<string, int, hasher, equality> components;
		std::unordered_map<int, string> reverse_components;

		std::unordered_map<string, int, hasher, equality> items;
		std::unordered_map<int, int> lineToItem;

//		data::item::item item;
		//while (task.message.items.get(item))
		for(int i=0L;i<task.message.items.count();++i)
		{
			data::item::item item = task.message.items[i];
			items[item.name] = item.itemID;
		};

		int in_count = 0, out_count = 0;
		//data::line::line line;
		//while (task.message.lines.get(line))
		for(int i=0;i<task.message.lines.count();++i)
		{
			data::line::line line = task.message.lines[i];
			lineToType[line.lineID] = line.typeID;
			lineToItem[line.lineID] = line.itemID;

			if (line.typeID == 0) ++in_count;
			else ++out_count;
		};

		int component_count = 0;
		//int component_index = 0;

		// componentID may be wrong here..??
		// needs to reset to zero per new item
		//data::component::component component;
		//while (task.message.components.get(component))
		for(int i=0;i<task.message.components.count();++i)
		{
			data::component::component component = task.message.components[i];

			if (components.find(component.name) == components.end())
			{
				components[component.name] = component_count;
				reverse_components[component.componentID] = component.name;
				componentToLine[component.componentID] = component.lineID;
				++component_count;
			}
		};

		int indices[255];
		for (int i = 0; i < 255; ++i) indices[i] = 0;

		std::unordered_map<string, int, hasher, equality> elements[255];
		//data::element::element element;

		//while (task.message.elements.get(element))
		for(int i=0;i<task.message.elements.count();++i)
		{
			data::element::element element = task.message.elements[i];

			if(elements[element.componentID].find(element.value) == elements[element.componentID].end())
				(elements[element.componentID])[element.value] = indices[element.componentID]++;
		}

		// for each in line
		// minus out line

		// per item, total calculations
		// in.lines.count * out.lines.count * 
		grid in, out[10];
		int in_index = 0, out_index = 0;

		//data::element::element e;
		//data::component::component c;
		//while (task.message.components.get(c))
		//while(task.message.elements.get(e))
		for(int i=0;i<task.message.elements.count();++i)
		{
			data::element::element e = task.message.elements[i];

			int lineID = componentToLine[e.componentID];
			int type = lineToType[lineID];
			if (type == 0) // in
			{
				//reverse_components[e.componentID]
				int idx_x = (in_index * component_count) + components[e.componentID];
				int idx_y = (elements[e.componentID])[e.value];
				
				//in.headers[idx_x].messageID = task.message.messageID;
				//in.headers[idx_x].itemID = lineToItem[lineID];
				//in.headers[idx_x].lineID = lineID;

				//in.data[idx_x][idx_y] = 1;
				//in.data[(idx_y * 255) + idx_x] = 1;
				in.set(idx_x, idx_y);

				++in_index;
			}
			else
			{
				for (int i = 0; i < in_count; ++i)
				{
					int idx_x = (i * component_count) + components[e.componentID];
					int idx_y = (elements[e.componentID])[e.value];

					//out[out_index].headers[idx_x].messageID = task.message.messageID;
					//out[out_index].headers[idx_x].itemID = lineToItem[lineID];
					//out[out_index].headers[idx_x].lineID = lineID;

					// eachforach acuquired, put out in each acuiqred slot
					//out[out_index].data[idx_x][idx_y] = 1;
					//out[out_index].data[(idx_y * 255) + idx_x] = 1;
					out->set(idx_x, idx_y);
				}

				++out_index;
			}

			//int index = c.lineID
		};
		

		for (int i = 0; i < out_index; ++i)
		{
			in.minus(out[i]);
		}

		for (int i = 0; i < task.message.queries.count(); ++i)
		{
			data::query::query query = task.message.queries[i];

			//block q;

			// populate block with current query
			// then compare with in block!!!!

		}

		// NEXT LOOP THROUGH OUTPUTS
		// AND SUBTRACT FROM INPUTS

		// THEN LOOP THROUGH QUERIES
		// TO COMPARE
		// THEN OUTPUT VALID ACQUIRED LINES
		// THEN COMBINE IF CONTINIOUS DATE RANGE

		// create hashes foreach item component

		//};
		/*
		data::line::line line;
		while (task.message.lines.get(line))
		{
			if (line.typeID == 0)
			{
				// in
			}
			else if (line.typeID == 1)
			{
				// out
			}
		};
		*/
		// index = [itemID * max_components];
		//data::item::item item;
		//while (task.message.items.get(item))
		//{
			
			// create hashes foreach item component
			
		//};
	}
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