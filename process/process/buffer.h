#include "string.h"
#include "log.h"

#if !defined(__BUFFER)
#define __BUFFER

class buffer
{
	const static long default_length = 8192L;

	long lpwrite, length;
	char *data;

	bool init;

public:
	bool wrap;

public:
	buffer() { makeNull(); reset(default_length); }
	buffer(long len) { makeNull(); reset(len); }
	~buffer() { cleanup(); }

	bool initalised() { return init; }
	void reset(long len);

	void clear();

	long indexOf(string &source);
	long indexOf(const char *source, long src_length);

	bool put(char character);

	bool put(string &source);
	bool put(const char *character, long length);

	bool get(long index, char &character);
	string get(long index, long length);

	long count() { return lpwrite; }

	bool isfull() { return lpwrite == length; }

	bool copy(const buffer &source);

	void output() { Log << data; }

public:
	buffer& operator=(const buffer& source)
	{
		this->copy(source);
		return *this;
	}

protected:
	void makeNull();
	void cleanup();
};

#endif