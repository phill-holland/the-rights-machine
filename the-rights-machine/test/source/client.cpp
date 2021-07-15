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
    if(!in.is_open()) return;

    in.seekg(0, std::ios::end);
    //std::streampos end = in.tellg();
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
    //std::cout << "post\n" << temp << "\n";

    //web::page destination;
    destination->url = url;
    destination->port = port;
    
    return cli.post(destination, &source);

    //std::cout << destination.data() << "\n";
// decode destination page into responses
// put into std::vector
// streaming output!
// cli.post(include parser for result)
    //std::cout << destination.data();
    //return result;
}

void test::client::makeNull()
{
    buffer = NULL;
}

void test::client::cleanup()
{
    if(buffer != NULL) delete[] buffer;
}