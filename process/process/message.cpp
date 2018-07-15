#include "message.h"
#include "log.h"
//#include <iostream>

/*
void data::message::base::clear()
{
	messageID = 0;
	userID = 0;
	created.clear();
	finished.clear();
}

void data::message::base::copy(base const &source)
{
	messageID = source.messageID;
	userID = source.userID;
	created = source.created;
	finished = source.finished;
}

bool data::message::base::add(custom::pair &source)
{
	if (string("userID").icompare(source.name))
	{
		userID = source.value.toInteger();
		return true;
	}

	return false;
}
*/
void data::message::message::reset()
{
	init = false; cleanup();

	
	items.parent(this);
	queries.parent(this);
	lines.parent(&items);// .temp);
	components.parent(&lines);// .temp);
	elements.parent(&components);// .temp);
	
	/*
	components.parent(&(lines.temp));
	components.temp.parent(&components);
	elements.parent(&(components.temp));
	elements.temp.parent(&elements);
	*/
	json *identifiers[] = { &elements.temp, &elements, &components.temp, &components, &lines.temp, &lines, &items.temp, &items, &queries.temp, &queries, this };
	for (long i = 0L; i < 11L; ++i)
	{
		Log << "ME " << identifiers[i]->identifier() << "\r\n";
		Log << "FQDN " << identifiers[i]->FQDN() << "\r\n";

		hash[identifiers[i]->FQDN()] = identifiers[i];
	}

	queue::base *queues[] = { &queries, &elements, &components, &lines, &items };
	json *id[] = { &queries, &elements, &components, &lines, &items };

	for (long i = 0L; i < 5L; ++i)
	{
		queue_hash[id[i]->FQDN()] = queues[i];
	}
	/*
	hash[this->FQDN()] = this;
	hash[items.FQDN()] = &items;
	hash[items.temp.FQDN()] = &(items.temp);
	hash[lines.FQDN()] = &lines;
	hash[lines.temp.FQDN()] = &(lines.temp);
	hash[components.FQDN()] = &components;
	hash[components.temp.FQDN()] = &(components.temp);
	hash[elements.identifier()] = &elements;
	hash[elements.temp.identifier()] = &(elements.temp);
	*/
	/*
	hash[this->identifier()] = this;
	hash[items.identifier()] = &items;
	hash[items.temp.identifier()] = &(items.temp);
	hash[lines.identifier()] = &lines;
	hash[lines.temp.identifier()] = &(lines.temp);
	hash[components.identifier()] = &components;
	hash[components.temp.identifier()] = &(components.temp);
	hash[elements.identifier()] = &elements;
	hash[elements.temp.identifier()] = &(elements.temp);
	*/
	//items = new data::items::items<MAX>();
	//if (items == NULL) return;
	//if (!items->initalised()) return;

	clear();

	init = true;
}

void data::message::message::clear()
{
	messageID = 0;
	userID = 0;
	created.clear();
	finished.clear();

	queries.clear();
	items.clear();
	lines.clear();
	components.clear();
	elements.clear();
	//base::clear();
	//items->empty();
}

queue::base *data::message::message::findQ(string FQDN)
{
	std::unordered_map<string, queue::base*, hasher, equality>::iterator i = queue_hash.find(FQDN);
	if (i != queue_hash.end()) return (queue::base*)i->second;

	return NULL;
}

data::json *data::message::message::find(string FQDN)
{
	std::unordered_map<string, data::json*, hasher, equality>::iterator i = hash.find(FQDN);
	if(i != hash.end()) return (data::json*)i->second;

	return NULL;
	//{
	//	return (data::json*)i->second;
	//}
}

//data::json *data::message::message::find(string label)
//{
	//data::json *result = NULL;
	// after comma, closing bracket, or square bracket
	// add type to temp message queue
	// until we run out of } brackets
	// then pump the whole lot out

	// COMMA IS DIFFERENT -- may represent just a value
	//if (current != NULL)
	//{
	//	if (current->identifier().icompare(label)) return current;
	//}

	/*
	items.parent(&message);
	item.parent(&items);
	lines.parent(&item);
	line.parent(&lines);
	components.parent(&line);
	component.parent(&components);
	*/

	// hashset<data::json*>
	/*
	struct equality 
	{
	public:
		bool operator()(const string &str1, const string &str2) const 
		{
			return ((string&)str1).icompare((string&)str2);
		}
	};
	
	std::unordered_map<string, data::json *, equality> hash;
	*/
	/*
	data::json *classes[] = { this,
							  &items,
							  &(items.temp),
							  &lines,
							  &(lines.temp),
							  &components,
							  &(components.temp),
							  &elements,
							  &(elements.temp) };
							  */


		/*
	for (long i = 0L; i < 9L; ++i)
	{
		hash[classes[i]->identifier().hash()] = classes[i];
	}
	*/
	//string k = string("MESSAGE");
	//string m = k.upper();
	//long h = m.hash();

	//std::size_t moo = (std::size_t)(((string&)k).upper()).hash();

	//std::unordered_map<string, data::json*, equality>::iterator i = hash.find(label);
	//if (i != hash.end()) return (data::json*)i->second;

	/*
	for (long i = 0L; i < 9L; ++i)
	{
		if (classes[i]->identifier().icompare(label))
		{
			classes[i]->clear();
			return classes[i];
		}
	}
	*/
	//return NULL;
//}

void data::message::message::copy(message const &source)
{
	messageID = source.messageID;
	userID = source.userID;
	created = source.created;
	finished = source.finished;

	items = source.items;
	lines = source.lines;
	components = source.components;
	elements = source.elements;	
}

bool data::message::message::add(custom::pair &source)
{
	if (string("userID").icompare(source.name))
	{
		userID = source.value.toInteger();
		return true;
	}

	return false;
}

void data::message::message::makeNull()
{
	//items = NULL;
}

void data::message::message::cleanup()
{
	//if (items != NULL) delete items;
}