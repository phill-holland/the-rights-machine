#include "log.h"
#include <memory.h>
#include <stdlib.h>
#include <stdio.h>
#include <cstring>

#ifdef _LDEBUG

long logger::constructed = 0L;
std::ofstream logger::handle;

logger::logger()
{
	if(constructed==0UL) reset("log.txt");

	++constructed;
}

logger::logger(char *destination)
{
	reset(destination);

	++constructed;
}

void logger::add(char *line)
{
	write(line);
}

void logger::add(unsigned char *line)
{
	write((char*)line);
}

void logger::add(float number)
{
	char temp[size];
	floatToStr(temp, number);
	write(temp);
}

void logger::add(long number)
{
	char temp[size];
	longToStr(temp, number);
	write(temp);
}

void logger::add(int number)
{
	char temp[size];
	intToStr(temp, number);
	write(temp);
}

void logger::add(bool value)
{
	char temp[size];
	boolToStr(temp, value);
	write(temp);
}

void logger::add_hex(long number)
{
	char temp[size];
	longHexToStr(temp, number);
	write(temp);
}

void logger::add(string &line)
{
	write(line.c_str());
}

void logger::reset(const char *filename)
{
	handle.open(filename, std::ios::out | std::ios::trunc | std::ios::binary);
}

void logger::write(const char *line)
{
	semaphore lock(token);

	handle.write(line, strlen(line));
	handle.flush();
}

void logger::floatToStr(char *temp, float number)
{
	memset(temp, 0, size);
	snprintf(temp, size, "%2.8f", number);
}

void logger::intToStr(char *temp, int number)
{
	memset(temp, 0, size);
	snprintf((char*)temp, 10, "%d", number);
}

void logger::longToStr(char *temp, long number)
{
	memset(temp, 0, size);
	snprintf((char*)temp, 10, "%ld", number);
}

void logger::longHexToStr(char *temp, long number)
{
	memset(temp, 0, size);
	snprintf(temp, size, "0x%2x", (unsigned int)number);
}

void logger::boolToStr(char *temp, bool value)
{
	memset(temp, 0, size);

	if (value) memcpy(temp, "true\0", 5);
	else memcpy(temp, "false\0", 6);
}

#endif

logger Log;

