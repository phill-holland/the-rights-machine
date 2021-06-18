#include "stdafx.h"
#include "error.h"

tests::data::error::error()
{
	json *identifiers[] = { this };

	for (long i = 0L; i < 1L; ++i)
	{
		hash[identifiers[i]->FQDN()] = identifiers[i];
	}
}

bool tests::data::error::add(custom::pair source)
{
	if (string("code").icompare(source.name))
	{
		code = source.value.toLong();
	}
	if (string("name").icompare(source.name))
	{
		name = source.value;

		return true;
	}
	else if (string("description").icompare(source.name))
	{
		description = source.value;

		return true;
	}
	
	return false;
}

::data::json::request::json *tests::data::error::find(string FQDN)
{
	std::unordered_map<string, json*, comparison::hasher, comparison::equality>::iterator i = hash.find(FQDN);
	if (i != hash.end()) return (json*)i->second;

	return NULL;
}