#include "message.h"
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
	lines.parent(&items.temp);
	components.parent(&lines.temp);
	elements.parent(&components.temp);

	
	hash[this->identifier()] = this;
	hash[items.identifier()] = &items;
	hash[items.temp.identifier()] = &(items.temp);
	hash[lines.identifier()] = &lines;
	hash[lines.temp.identifier()] = &(lines.temp);
	hash[components.identifier()] = &components;
	hash[components.temp.identifier()] = &(components.temp);
	hash[elements.identifier()] = &elements;
	hash[elements.temp.identifier()] = &(elements.temp);
	
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

	items.clear();
	lines.clear();
	components.clear();
	elements.clear();
	//base::clear();
	//items->empty();
}

data::json *data::message::message::find(string label)
{
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
	std::unordered_map<string, data::json*, equality>::iterator i = hash.find(label);
	if (i != hash.end()) return (data::json*)i->second;

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
	return NULL;
}

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