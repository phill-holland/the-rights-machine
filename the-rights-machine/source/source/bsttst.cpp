#include "bsttst.h"
#include <iostream>
#include <fstream>
#include <cstring>
//#include "custom/string.h"

void bsttst::bsttst::go(std::string filename)
{
    //std::string filename;

     std::ifstream in(filename, std::ios::binary);
    if(!in.is_open()) return;

    in.seekg(0, std::ios::end);
    std::streampos end = in.tellg();
    in.seekg(0, std::ios::beg);

    char *buffer = new char[end];
    if(buffer == NULL) return;
    memset(buffer, 0, end);

    in.read(buffer, end);
    in.close();

// ****
    boost::json::error_code ec;
    write(buffer, end, ec);
}