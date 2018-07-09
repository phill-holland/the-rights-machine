#include "element.h"

void data::element::element::clear()
{
	elementID = 0;
	componentID = 0;
	value = "";
}

bool data::element::element::add(custom::pair &source)
{
	if (string("value").icompare(source.name))
	{
		value = source.value;
		return true;
	}

	return false;
}

void data::element::element::copy(element const &source)
{
	elementID = source.elementID;
	componentID = source.componentID;
	value = source.value;
}