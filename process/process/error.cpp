#include "error.h"
#include <stdlib.h>

void error::type::types::reset()
{
	init = false; cleanup();

	length = 0UL;

	errors = new type[LENGTH];
	if (errors == NULL) return;

	init = true;
}

bool error::type::types::push(type &t)
{
	if (length >= LENGTH) return false;
	if (reverse.find(t.name) != reverse.end()) return false;

	t.code = length + 1L;
	errors[length++] = t;

	reverse[t.name] = t.code;

	return true;
}

error::type::type error::type::types::lookup(::error::error &error)
{
	::error::type::type result;
	if (reverse.find(error.name) == reverse.end())  return result;

	long code = reverse[error.name] - 1L;
	if ((code >= 0L) && (code < (long)length)) result = errors[code];

	return result;
}

void error::type::types::makeNull()
{
	errors = NULL;
}

void error::type::types::cleanup()
{
	if (errors != NULL) delete errors;
}
