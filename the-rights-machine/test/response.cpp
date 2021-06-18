#include "stdafx.h"
#include "response.h"

tests::data::response::response()
{
	json *identifiers[] = { this };

	for (long i = 0L; i < 1L; ++i)
	{
		hash[identifiers[i]->FQDN()] = identifiers[i];
	}
}

bool tests::data::response::add(custom::pair source)
{
	if (string("guid").icompare(source.name))
	{
		GUID = source.value;
		return true;
	}

	return false;
}

::data::json::request::json *tests::data::response::find(string FQDN)
{
	std::unordered_map<string, json*, comparison::hasher, comparison::equality>::iterator i = hash.find(FQDN);
	if (i != hash.end()) return (json*)i->second;

	return NULL;
}