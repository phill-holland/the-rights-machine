#include "manager.h"

void manager::manager::reset(queue::factory <data::response> *factory)
{
	init = false; cleanup();

	read = 0L;

	this->factory = factory;

	init = true;
}

void manager::manager::add(compute::compute *source)
{
	mutex lock(token);

	nodes.push_back(source);
}

bool manager::manager::set(compute::task &source)
{
	long count = 0L;

	mutex lock(token);

	bool result = false;
	do
	{
		if (read >= (long)nodes.size()) read = 0L;
		result = nodes[read++]->set(source);
		++count;
	} while ((!result)&&(count < nodes.size()));

	return result;
}

bool manager::manager::get(::queue::queue<data::response> &destination)
{
	::queue::queue<data::response> *temp = factory->get();
	if (temp == NULL) return false;
	destination = *temp;

	return true;
}

void manager::manager::makeNull()
{
}

void manager::manager::cleanup()
{
}