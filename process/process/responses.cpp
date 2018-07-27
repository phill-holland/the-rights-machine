#include "responses.h"

void data::response::responses::reset()
{
	init = false; cleanup();

	clear();

	init = true;
}
