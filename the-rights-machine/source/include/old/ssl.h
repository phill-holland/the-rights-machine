#include <openssl/rand.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include "core/string/string.h"

#ifndef _SSL
#define _SSL

class ssl
{
	static int constructed;
	static bool init;

protected:
	SSL *sslHandle;
	SSL_CTX *sslContext;

public:
	ssl();
	~ssl();

	bool initalised() { return init; }

	bool open(int _socket);

	int read(char *buffer, long length);
	int write(string &data);

	void close();
};

#endif