#include "stdafx.h"
#include "result.h"

tests::data::result::result()
{
	json *identifiers[] = { this };

	for (long i = 0L; i < 1L; ++i)
	{
		hash[identifiers[i]->FQDN()] = identifiers[i];
	}
}

bool tests::data::result::add(custom::pair &source)
{
	if (string("guid").icompare(source.name))
	{
		GUID = source.value;
		return true;
	}
	else if (string("userid").icompare(source.name))
	{
		userID = source.value.toInteger();
		return true;
	}
	else if (string("queryid").icompare(source.name))
	{
		queryID = source.value.toInteger();
		return true;
	}
	else if (string("created").icompare(source.name))
	{
		created = (datetime)source.value;
		return true;
	}
	else if (string("available").icompare(source.name))
	{
		available = source.value.toBoolean();
		return true;
	}

	return false;
}

::data::json::request::json *tests::data::result::find(string FQDN)
{
	std::unordered_map<string, json*, comparison::hasher, comparison::equality>::iterator i = hash.find(FQDN);
	if (i != hash.end()) return (json*)i->second;

	return NULL;
}