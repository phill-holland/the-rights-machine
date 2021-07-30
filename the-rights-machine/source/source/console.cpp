#include "console.h"
#include <memory.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>

void debug::console::background(thread *bt)
{
    std::chrono::high_resolution_clock::time_point now = std::chrono::high_resolution_clock::now();
    if((now >= clock)||(data->entries() > (QUEUE - BUFFER)))
    {
        flush();
        clock = std::chrono::high_resolution_clock::now() + std::chrono::minutes(minutes);
    }

    sleep(5000);
}

void debug::console::reset(string filename)
{
    init = false; cleanup();

    this->filename = filename;
    this->minutes = MINUTES;

    data = new fifo<string, QUEUE>();
    if (data == NULL) return;
    if (!data->initalised()) return;

    std::ofstream handle;
    handle.open(filename, std::ios::out | std::ios::trunc | std::ios::binary);
    handle.close();

    clock = std::chrono::high_resolution_clock::now() + std::chrono::minutes(minutes);

    init = this->start();
}

bool debug::console::push(string source, bool timestamp)
{
    if (timestamp)
	{
		time_t now = time(NULL);
		string temp;
		temp.concat(string::fromTime(now));
		temp.concat(string("> "));
		temp.concat(source);
        temp.concat(string("\r\n"));

		return data->set(temp);
	}
	else return data->set(source);
}

bool debug::console::push(float source)
{
    string temp = string::fromFloat(source);
    return data->set(temp);
}

bool debug::console::push(long source)
{
    string temp = string::fromLong(source);
    return data->set(temp);
}

bool debug::console::push(int source)
{
    string temp = string::fromInt(source);
    return data->set(temp);
}

bool debug::console::push(bool value)
{
    string temp = string::fromBool(value);
    return data->set(temp);
}

void debug::console::flush()
{
    unsigned long entries = data->entries();
    if(entries > 0UL)
    {
        semaphore lock(token);

        for(unsigned long i = 0UL; i < entries; ++i)
        {
            string temp;
            if(data->get(temp))
            {
                std::cout << temp;
            }
        }
    }
}

void debug::console::makeNull()
{
    data = NULL;
}

void debug::console::cleanup()
{
    if(data != NULL) delete data;
}