#include "ssl.h"

int ssl::constructed = 0;
bool ssl::init = false;

ssl::ssl()
{
	sslHandle = NULL;
	sslContext = NULL;

	if(constructed==0)
	{
		SSL_load_error_strings();
		SSL_library_init();

		init = true;
	}

	++constructed;
};

ssl::~ssl()
{
	--constructed;
}

bool ssl::open(int _socket)
{
	sslContext = SSL_CTX_new(SSLv23_client_method());
	if(sslContext == NULL) return false;
	sslHandle = SSL_new(sslContext);

	if(sslHandle == NULL) return false;
	if(!SSL_set_fd(sslHandle, _socket)) return false;

	if(SSL_connect(sslHandle) != 1) return false;

	return true;
}

int ssl::read(char *buffer, long length)
{
	return SSL_read(sslHandle,buffer,length);
}

int ssl::write(string &data)
{
	int result = SSL_write(sslHandle, data.c_str(), data.count());

	return result;
}

void ssl::close()
{
	if(sslHandle != NULL)
	{
		SSL_shutdown(sslHandle);
		SSL_free(sslHandle);
	}

	if(sslContext != NULL)
	{
		SSL_CTX_free(sslContext);
	}
}