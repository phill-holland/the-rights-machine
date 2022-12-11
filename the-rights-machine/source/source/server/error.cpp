#include "server/error.h"
#include <stdlib.h>

unsigned long error::type::type::pairs()
{
	return 3UL;
}

core::custom::pair error::type::type::pull(unsigned long index)
{
	core::custom::pair result;

	if (index == 0UL) result = core::custom::pair(string("code"), string::fromLong(code));
	else if (index == 1UL) result = core::custom::pair(string("name"), name);
	else if (index == 2UL) result = core::custom::pair(string("description"), description);

	return result;
}

void error::type::types::reset(queue::out<::error::type::type> *source)
{
	init = false; cleanup();

	length = 0UL;
	
	errors = new type*[LENGTH];
	if (errors == NULL) return;
	for (unsigned long i = 0UL; i < LENGTH; ++i) errors[i] = NULL;
	
	for (unsigned long i = 0UL; i < LENGTH; ++i)
	{
		errors[i] = new type();
		if (errors[i] == NULL) return;
	}
	
	if (source != NULL)
	{
		::error::type::type temp;
		while (source->get(temp))
		{
			push(temp);
		}
	}

	init = true;
}

bool error::type::types::push(type &t)
{
	if (length >= LENGTH) return false;
	if (reverse.find(t.name) != reverse.end()) return false;

	t.code = length + 1L;
	*errors[length++] = t;

	reverse[t.name] = t.code;

	return true;
}

error::type::type error::type::types::lookup(::error::error &error)
{
	::error::type::type result;
	result.name = error.name;

	if (reverse.find(error.name) == reverse.end())  return result;

	long code = reverse[error.name] - 1L;
	if ((code >= 0L) && (code < (long)length)) result = *errors[code];
	
	return result;
}

void error::type::types::makeNull()
{
	errors = NULL;
}

void error::type::types::cleanup()
{
	if (errors != NULL)
	{
		for (long i = (LENGTH - 1L); i >= 0L; i--)
		{
			if (errors[i] != NULL) delete errors[i];
		}

		delete errors;
	}
}
