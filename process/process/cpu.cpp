#include "cpu.h"
#include "string.h"
#include <unordered_map>

void compute::cpu::cpu::sort(datetime &a, datetime &b, datetime &c, datetime &d)
{
	auto swap = [](datetime &a, datetime &b)
	{
		datetime temp = a;
		a = b;
		b = temp;
	};

	int swaps = 0;

	do
	{
		swaps = 0;
		if (b < a) { swap(a, b); ++swaps; }
		if (c < b) { swap(b, c); ++swaps; }
		if (d < c) { swap(c, d); ++swaps; }
	} while (swaps > 0);
}

DWORD WINAPI compute::cpu::cpu::background(thread *bt)
{
	::compute::task task;
	if (this->get(task))
	{
		// check overlaps
		// with dates here

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
		data::line::line ln_in;
		while (task.message.lines.get(ln_in))
		{
			if (ln_in.typeID == 0)
			{
				data::line::line ln_out;
				bool overlap = false;
				while (task.message.lines.get(ln_out))
				{
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

								sort(a, b, c, d);

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
				};

				if (!overlap)
				{
					lines[ln_out_ptr++] = ln_in;
				}
			}
		};

		// USE LINES ARRAY INSTEAD 
		// still need to consider how to map components to new lines

		std::unordered_map<int, int> lineToType;
		std::unordered_map<int, int> componentToLine;

		std::unordered_map<string, int> components;
		std::unordered_map<int, string> reverse_components;

		std::unordered_map<string, int> items;
		std::unordered_map<int, int> lineToItem;

		data::item::item item;
		while (task.message.items.get(item))
		{
			items[item.name] = item.itemID;
		};

		int in_count = 0, out_count = 0;
		data::line::line line;
		while (task.message.lines.get(line))
		{
			lineToType[line.lineID] = line.typeID;
			lineToItem[line.lineID] = line.itemID;

			if (line.typeID == 0) ++in_count;
			else ++out_count;
		};

		int component_count = 0;
		//int component_index = 0;

		// componentID may be wrong here..??
		// needs to reset to zero per new item
		data::component::component component;
		while (task.message.components.get(component))
		{
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

		std::unordered_map<string, int> elements[255];
		data::element::element element;

		while (task.message.elements.get(element))
		{
			if(elements[element.componentID].find(element.value) == elements[element.componentID].end())
				(elements[element.componentID])[element.value] = indices[element.componentID]++;
		}

		// for each in line
		// minus out line

		// per item, total calculations
		// in.lines.count * out.lines.count * 
		block in, out[10];
		int in_index = 0, out_index = 0;

		data::element::element e;
		//data::component::component c;
		//while (task.message.components.get(c))
		while(task.message.elements.get(e))
		{
			int lineID = componentToLine[e.componentID];
			int type = lineToType[lineID];
			if (type == 0) // in
			{
				//reverse_components[e.componentID]
				int idx_x = (in_index * component_count) + components[e.componentID];
				int idx_y = (elements[e.componentID])[e.value];
				
				in.headers[idx_x].messageID = task.message.messageID;
				in.headers[idx_x].itemID = lineToItem[lineID];
				in.headers[idx_x].lineID = lineID;

				in.data[idx_x][idx_y] = 1;

				++in_index;
			}
			else
			{
				for (int i = 0; i < in_count; ++i)
				{
					int idx_x = (i * component_count) + components[e.componentID];
					int idx_y = (elements[e.componentID])[e.value];

					out[out_index].headers[idx_x].messageID = task.message.messageID;
					out[out_index].headers[idx_x].itemID = lineToItem[lineID];
					out[out_index].headers[idx_x].lineID = lineID;

					// eachforach acuquired, put out in each acuiqred slot
					out[out_index].data[idx_x][idx_y] = 1;
				}

				++out_index;
			}

			//int index = c.lineID
		};
		
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