#include "message/inquiry.h"

void data::message::inquiry::reset()
{
	init = false; cleanup();

	queries.json::parent(this);

	queries.temp.components.json::parent(&queries);
	queries.temp.elements.json::parent(&queries.temp.components);

	json *identifiers[] = { &queries.temp.elements.temp, &queries.temp.elements,
							&queries.temp.components.temp, &queries.temp.components,
							&queries.temp, &queries, this };

	for (long i = 0L; i < 7L; ++i)
	{
		hash[identifiers[i]->FQDN()] = identifiers[i];
	}
	
	queue::base *queues[] = { &queries, &queries.temp.elements, &queries.temp.components };
	json *id[] = { &queries, &queries.temp.elements, &queries.temp.components };

	for (long i = 0L; i < 3L; ++i)
	{
		queue_hash[id[i]->FQDN()] = queues[i];
	}

	clear();

	init = true;
}

void data::message::inquiry::clear()
{
	queries.clear();
}

queue::base *data::message::inquiry::findQ(string FQDN)
{
	std::unordered_map<string, queue::base*, hasher, equality>::iterator i = queue_hash.find(FQDN);
	if (i != queue_hash.end()) return (queue::base*)i->second;

	return NULL;
}

json::request::json *data::message::inquiry::find(string FQDN)
{
	std::unordered_map<string, ::json::request::json*, hasher, equality>::iterator i = hash.find(FQDN);
	if(i != hash.end()) return (::json::request::json*)i->second;

	return NULL;
}

void data::message::inquiry::copy(inquiry const &source)
{
	queries = source.queries;
}

string data::message::inquiry::output()
{
	string result("\"inquiry\" : {\r\n");
	for (long i = 0L; i < queries.count(); ++i) result.concat(queries[i]->output());
	result.concat(string("}\r\n"));

	return result;
}

bool data::message::inquiry::add(core::custom::pair source)
{
	/*
	if (string("user").icompare(source.name))
	{
		user = source.value;
		return true;
	}
	else if (string("apikey").icompare(source.name))
	{
		apikey = source.value;
		return true;
	}
	*/

	return false;
}

void data::message::inquiry::makeNull()
{
}

void data::message::inquiry::cleanup()
{
}