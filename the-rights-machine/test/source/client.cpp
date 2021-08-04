#include "test/client.h"
#include <iostream>
#include <fstream>
#include <cstring>

//#include <iostream>

void test::client::reset(string filename)
{
    init = false; cleanup();
    length = 0;

    std::ifstream in(filename, std::ios::binary);
    if(in.fail()) return;
    if(!in.is_open()) return;

    in.seekg(0, std::ios::end);
    length = in.tellg();

    in.seekg(0, std::ios::beg);

    buffer = new char[length];
    if(buffer == NULL) return;
    memset(buffer, 0, length);

    in.read(buffer, length);
    in.close();

    init = true;
}

bool test::client::post(string url, long port, web::page *destination)
{
    string temp(buffer, length);

    source.data(temp);

    destination->url = url;
    destination->port = port;
    
    return cli.post(destination, &source);
}

void test::client::makeNull()
{
    buffer = NULL;
}

void test::client::cleanup()
{
    if(buffer != NULL) delete[] buffer;
}