#include "component.h"

void data::component::component::clear()
{
	componentID = 0;
	lineID = 0;
	name = "";
}

bool data::component::component::add(custom::pair &source)
{
	if (string("name").icompare(source.name))
	{
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
}