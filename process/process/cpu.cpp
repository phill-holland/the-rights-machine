#include "cpu.h"

// has many worker threads for grid subtraction

DWORD WINAPI compute::cpu::background(thread *bt)
{
	return (DWORD)0;
}

void compute::cpu::reset(::queue::factory<::compute::task> *factory)
{
	init = false; cleanup();

	queue = factory->get();
	if (queue == NULL) return;

	init = true;
}

void compute::cpu::makeNull()
{
}

void compute::cpu::cleanup()
{
}

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