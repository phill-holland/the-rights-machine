#include "models/responses.h"
#include "core/custom/datetime.h"

void models::response::responses::background(thread *bt)
{
	core::threading::mutex lock(token);

	std::vector<string> erase;

	for (unsigned long i = 0UL; i < length; ++i)
	{
		models::response::response temp = *data[i];
		if ((temp.created + core::custom::datetime(0,0,0,0,30,0)) < core::custom::datetime::now())
		{
			erase.push_back(temp.guid);
		}
	}

	for (unsigned long i = 0UL; i < (unsigned long)erase.size(); ++i)
	{
		remove(erase[i]);
	}

	sleep(250);
}

void models::response::responses::reset(unsigned long total)
{
	core::threading::mutex lock(token);

	init = false; cleanup();

	this->total = total;
	length = 0UL;

	data = new models::response::response*[total];
	if (data == NULL) return;
	for (unsigned long i = 0UL; i < total; ++i) data[i] = NULL;

	for (unsigned long i = 0UL; i < total; ++i)
	{
		data[i] = new models::response::response();
		if (data[i] == NULL) return;
	}

	map.clear();

	init = true;
}

void models::response::responses::clear() 
{ 
	core::threading::mutex lock(token);

	length = 0UL;
	map.clear();
}

bool models::response::responses::set(models::response::response &source)
{
	core::threading::mutex lock(token);

	if (length >= total) return false;
	if (map.find(source.guid) != map.end()) return false;

	*data[length] = source;
	map[source.guid] = length;
	++length;

	return true;
}

models::response::response models::response::responses::get(unsigned long index)
{
	core::threading::mutex lock(token);

	return *data[index];
}

unsigned long models::response::responses::count() 
{ 
	core::threading::mutex lock(token);

	return length;
}

models::response::response models::response::responses::find(string &identity)
{
	core::threading::mutex lock(token);

	models::response::response result;
	if (map.find(identity) != map.end())
	{
		result = *data[map[identity]];
	}

	return result;
}

bool models::response::responses::remove(string &identity)
{
	core::threading::mutex lock(token);

	auto it = map.find(identity);
	if (it != map.end())
	{
		data[map[identity]]->clear();
		map.erase(it);

		return true;
	}

	return false;
}

void models::response::responses::makeNull()
{
	data = NULL;
}

void models::response::responses::cleanup()
{
	if (data != NULL)
	{
		for (long i = (total - 1L); i >= 0L; i--)
		{
			if (data[i] != NULL) delete data[i];
		}
	}
}