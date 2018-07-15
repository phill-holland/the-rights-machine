#include "component.h"

void data::component::component::clear()
{
	componentID = 0;
	lineID = 0;
	//value = "";
	name = "";
}

bool data::component::component::add(custom::pair &source)
{
	if (string("name").icompare(source.name))
	{
		//value = source.value;
		name = source.value;
		return true;
	}

	return false;
}

void data::component::component::copy(component const &source)
{
	componentID = source.componentID;
	lineID = source.lineID;
	name = source.name;
	//value = source.value;
}

/*
void data::component::component::reset()
{
	init = false; cleanup();

	elements = new data::elements::elements<MAX>();
	if (elements == NULL) return;
	if (!elements->initalised()) return;

	clear();

	init = true;
}

void data::component::component::clear()
{
	base::clear();
	elements->empty();
}

void data::component::component::copy(component const &source)
{	
	base::copy(source);
	elements->copy(*source.elements);
}

void data::component::component::makeNull()
{
	elements = NULL;
}

void data::component::component::cleanup()
{
	if (elements != NULL) delete elements;
}
*/