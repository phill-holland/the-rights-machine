#include "parameters.h"

void web::parameters::reset()
{
	init = false; cleanup();

	clear();

	init = true;
}
