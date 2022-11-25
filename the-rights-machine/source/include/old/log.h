#include <stdio.h>
#include <iostream>
#include <fstream>
#include "core/string/string.h"
#include "thread.h"
#include "semaphore.h"

#ifndef _LOG
#define _LOG

#define _LDEBUG

#ifndef _LDEBUG

class logger
{
	static char tempString[15];

public:
	static char CRLF[3];

public:
    logger() { }
    logger(char *destination) { }
    void addln(char *line) { }
	void addln(float number) { }
	void addln(int number) { }
	void addln(long number) { }
	void add(char *line) { }
	void add(float number) { }
	void add(long number) { }
	void add_hex(long number) { }

public:
	logger operator<<(char *line) { return *this; }
	logger operator<<(float number) { return *this; }
	logger operator<<(int number) { return *this; }
	logger operator<<(long number) { return *this; }

	logger operator>>(char *line) { return *this; }
	logger operator>>(float number) { return *this; }
	logger operator>>(int number) { return *this; }
	logger operator>>(long number) { return *this; }

protected:
	static char *floatToStr(float number) { memset(tempString,0,sizeof(tempString); return tempString; }
	static char *intToStr(int number) { memset(tempString,0,sizeof(tempString); return tempString; }
	static char *longToStr(long number) { memset(tempString,0,sizeof(tempString); return tempString; }
	static char *longHexToStr(long number) { memset(tempString,0,sizeof(tempString); return tempString; }
};

extern logger Log;

#else

class logger
{
	static long constructed;
	static std::ofstream handle;

	static const long size = 25L;
	static const long buffer = 2048L;

	semaphore::token token;

public:

	logger();
    logger(char *destination);
	~logger() { if(constructed==-1L) handle.close(); --constructed; }

	void add(char *line);
	void add(unsigned char *line);
	void add(float number);
	void add(long number);
	void add(int number);
	void add(bool value);
	void add_hex(long number);
	void add(string &line);

	void now();

private:
    void reset(const char *filename);
	void write(const char *line);

public:
	logger operator<<(char *line) { add(line); return logger(); }
	logger operator<<(unsigned char *line) { add(line); return logger(); }
	logger operator<<(float number) { add(number); return logger(); }
	logger operator<<(double number) { add((float)number); return logger(); }
	logger operator<<(int number) { add(number); return logger(); }
	logger operator<<(long number) { add(number); return logger(); }
	logger operator<<(unsigned long number) { long temp = (long)number; add(temp); return logger(); }
	logger operator<<(bool value) { add(value); return logger(); }
	logger operator<<(string line) { add(line); return logger(); }

    logger operator>>(char *line) { add(line); return logger(); }
	logger operator>>(unsigned char *line) { add(line); return logger(); }
	logger operator>>(float number) { add(number); return logger(); }
	logger operator>>(double number) { add((float)number); return logger(); }
	logger operator>>(int number) { add(number); return logger(); }
	logger operator>>(long number) { add(number); return logger(); }
	logger operator>>(unsigned long number) { long temp = (long)number; add(temp); return logger(); }
	logger operator>>(bool value) { add(value); return logger(); }
	logger operator>>(string line) { add(line); return logger(); }

protected:
	void floatToStr(char *temp, float number);
	void intToStr(char *temp, int number);
	void longToStr(char *temp, long number);
	void longHexToStr(char *temp, long number);
	void boolToStr(char *temp, bool value);
};

extern logger Log;

#endif

#endif
