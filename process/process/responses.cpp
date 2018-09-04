#include "responses.h"
#include "datetime.h"
#include "log.h"

DWORD WINAPI data::response::responses::background(thread *bt)
{
	// check created date 
	Sleep(10000);
	
	mutex lock(token);

	std::vector<string> erase;

	for (unsigned long i = 0UL; i < count(); ++i)//(unsigned long)data.size(); ++i)
	{
		data::response::response temp = *data[i];
		if ((temp.created + global::datetime(0,0,0,0,30,0)) < global::datetime::now())//global::datetime.now())
		{
			erase.push_back(temp.GUID);
		}

	}

	for (unsigned long i = 0UL; i < (unsigned long)erase.size(); ++i)
	{
		remove(erase[i]);
	}

	return (DWORD)0;
}

void data::response::responses::reset(unsigned long total)
{
	mutex lock(token);

	init = false; cleanup();

	this->total = total;
	length = 0UL;

	data = new data::response::response*[total];
	if (data == NULL) return;
	for (unsigned long i = 0UL; i < total; ++i) data[i] = NULL;

	for (unsigned long i = 0UL; i < total; ++i)
	{
		data[i] = new data::response::response();
		if (data[i] == NULL) return;
	}
	//data.clear();
	map.clear();

	init = true;
}

void data::response::responses::clear() 
{ 
	mutex lock(token);

	length = 0UL;
	//data.clear(); 
	map.clear();
}

bool data::response::responses::set(data::response::response &source)
{
	mutex lock(token);

	if (count() >= total) return false;
	if (map.find(source.GUID) != map.end()) return false;

	*data[length] = source;
	//data.push_back(source);
	map[source.GUID] = length;//((unsigned long)data.size() - 1UL);
	++length;

/*
	if (count() >= MAX) return false;
	if (map.find(source.GUID) != map.end()) return false;

	data.push_back(source);
	map[source.GUID] = ((unsigned long)data.size() - 1UL);
	*/
	return true;
}

data::response::response data::response::responses::get(unsigned long index)
{
	mutex lock(token);

	return *data[index];
}

unsigned long data::response::responses::count() 
{ 
	mutex lock(token);

	return length;
	//return (unsigned long)data.size(); 
}

data::response::response data::response::responses::find(string &identity)
{
	Log << "find " << identity << "\r\n";
	mutex lock(token);

	data::response::response result;
	if (map.find(identity) != map.end())
	{
		result = *data[map[identity]];
	}

	return result;
}

bool data::response::responses::remove(string &identity)
{
	mutex lock(token);

	auto &it = map.find(identity);
	if (it != map.end())
	{
		//data.erase(data.begin() + map[identity]);
		data[map[identity]]->clear();
		map.erase(it);

		return true;
	}

	return false;
}

void data::response::responses::makeNull()
{
	data = NULL;
}

void data::response::responses::cleanup()
{
	if (data != NULL)
	{
		for (long i = (total - 1L); i >= 0L; i--)
		{
			if (data[i] != NULL) delete data[i];
		}
	}
}