#include "test/client.h"
#include <iostream>
#include <fstream>
#include <cstring>

//#include <iostream>

void test::client::reset(string filename)
{
    init = false; cleanup();

    std::ifstream in(filename, std::ios::binary);
    if(!in.is_open()) return;

    in.seekg(0, std::ios::end);
    std::streampos end = in.tellg();
    in.seekg(0, std::ios::beg);

    buffer = new char[end];
    if(buffer == NULL) return;
    memset(buffer, 0, end);

    in.read(buffer, end);
    in.close();

    init = true;
}

bool test::client::post(string url, long port)
{
    string temp(buffer);

    page.data(temp);

    web::page destination;
    destination.url = url;
    destination.port = port;
    
    bool result = cli.post(&destination, &page);

    //std::cout << destination.data();
    return result;
}

void test::client::makeNull()
{
    buffer = NULL;
}

void test::client::cleanup()
{
    if(buffer != NULL) delete[] buffer;
}