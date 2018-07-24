#include <typeinfo.h>
#include <string.h>

#if !defined(__ERROR)
#define __ERROR

typedef long errorCode;

const errorCode NO_ERR = 0L;
const errorCode UNKNOWN_ERR = 1L;
const errorCode ALLOCATION_ERR = 2L;
const errorCode GENERAL_ERR = 3L;
const errorCode XML_ERR = 4L;

const unsigned long errorStrLen = 512UL;
const unsigned long resultStrLen = errorStrLen * 2UL;

// have error as a threaded class, with a queue
// each time the timer ticks, pull out the data abd write to database

class errorLog
{
public:
	static char tempString[15];
	static char classString[errorStrLen];
	static char errorString[errorStrLen];

	static char resultString[resultStrLen];

	static errorCode lastError;

	static char *getLastErrorString();
};

template <class X> void setLastError(X *theClass, errorCode theError, char *errorStr) { setLastError(theClass, theError); strcpy_s(errorLog::errorString, 512UL, errorStr); }
template <class X> void setLastError(X *theClass, errorCode theError) { strcpy_s(errorLog::classString, 512UL, typeid(X).name()); errorLog::lastError = theError; }
template <class X> void setLastError(X *theClass) { setLastError(theClass, UNKNOWN_ERR); }

extern errorCode getLastError();
extern char *getLastErrorString();

#endif