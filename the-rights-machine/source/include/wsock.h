#include <winsock2.h>
#include <windows.h>
#include <stdio.h>
#include "string.h"

#if !defined(__WSOCK)
#define __WSOCK

#pragma comment (lib, "Ws2_32.lib")

namespace wsock
{
	class wsock
	{
		static int constructed;
		static bool init;

	protected:
		const static long IPV4_LEN = 16L;
		const static long IPV6_LEN = 46L;

	public:
		wsock();
		~wsock();

		bool initalised() { return init; }

		struct addrinfo *getHostByName(string &server);
	};

	class client : public wsock
	{
		friend class server;

	protected:
		SOCKET _socket;
		static const int timeout = 5000;

	public:
		client() : wsock() { _socket = INVALID_SOCKET; }

		bool open(string &source, long port);
		bool isopen() { return _socket != INVALID_SOCKET; }

		int read(char *buffer, int length, int flags);

		int write(string &data, int flags);
		int write(const char *buffer, int length, int flags);

		void close();
	};

	class server : public wsock
	{
	protected:
		SOCKET _socket;
		static const int timeout = 5000;

	public:
		server() : wsock() { _socket = INVALID_SOCKET; }
		~server() { close(); }

		bool open(long port);
		bool isopen() { return _socket != INVALID_SOCKET; }

		bool wait(client &destination);

		void close();
	};
};

#endif