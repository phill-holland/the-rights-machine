#include "log.h"
#include <stdlib.h>
#include <stdio.h>

#if defined(__LDEBUG)

long logger::constructed = 0L;
HANDLE logger::FileHandle;

logger::logger()
{
	if (constructed == 0UL) reset("log.txt");

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

void logger::reset(char *filename)
{
	FileHandle = CreateFile(filename, GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
}

void logger::write(const char *line)
{
	DWORD BytesWritten = (DWORD)0;

	mutex lock(token);

	WriteFile(FileHandle, line, (DWORD)strlen(line), &BytesWritten, NULL);
	FlushFileBuffers(FileHandle);
}

void logger::floatToStr(char *temp, float number)
{
	memset(temp, 0, size);
	sprintf_s(temp, size, "%2.8f", number);
}

void logger::intToStr(char *temp, int number)
{
	memset(temp, 0, size);
	_itoa_s(number, temp, size, 10);

}

void logger::longToStr(char *temp, long number)
{
	memset(temp, 0, size);
	_ltoa_s(number, temp, size, 10);
}

void logger::longHexToStr(char *temp, long number)
{
	memset(temp, 0, size);
	sprintf_s(temp, size, "0x%2x", number);
}

void logger::boolToStr(char * temp, bool value)
{
	memset(temp, 0, size);
	if (value) strcpy_s(temp, size, "true");
	else strcpy_s(temp, size, "false");
}

#endif

logger Log;

