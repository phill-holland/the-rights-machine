#include <windows.h>
#include <openssl/rand.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include "string.h"

#if !defined(__SSL)
#define __SSL

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

	bool open(SOCKET _socket);

	int read(char *buffer, long length);
	int write(string &data);

	void close();
};

#endif