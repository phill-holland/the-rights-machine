#include "error.h"
#include <stdlib.h>

char errorLog::errorString[errorStrLen] = { "" };
char errorLog::classString[errorStrLen] = { "" };

char errorLog::resultString[resultStrLen] = { "" };

errorCode errorLog::lastError = NO_ERR;

char *errorLog::getLastErrorString()
{
	memset(resultString, 0, resultStrLen);

	if (lastError != NO_ERR)
	{
		char tempString[15];
		memset(tempString, 0, 15);
		_ltoa_s(lastError, tempString, 15, 10);

		strcpy_s(resultString, resultStrLen, "ERR(");
		strcat_s(resultString, resultStrLen, tempString);
		strcat_s(resultString, resultStrLen, ",");

		char character = 0;
		unsigned long i = 0UL;

		strcat_s(resultString, resultStrLen, classString);

		if (strlen(errorString)>0)
		{
			strcat_s(resultString, resultStrLen, ",");
			strcat_s(resultString, resultStrLen, errorString);
		}
		strcat_s(resultString, resultStrLen, ")");
	}
	else strcpy_s(resultString, resultStrLen, "ERR(0)");

	return resultString;
}

errorCode getLastError() { return errorLog::lastError; }

char *getLastErrorString() { return errorLog::getLastErrorString(); }
