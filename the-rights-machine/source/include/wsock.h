#ifndef _WSOCK
#define _WSOCK

#ifdef WIN32

#include <winsock2.h>
#include <windows.h>
#include <stdio.h>
#include "custom/string.h"

#pragma comment (lib, "Ws2_32.lib")

namespace wsock
{
	class wsock
	{
	protected:
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
		const static int timeout = 5000;

	private:
		friend class server;

	protected:
		SOCKET _socket;

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
		const static int timeout = 5000;

	public:
		server() : wsock() { _socket = INVALID_SOCKET; }
		~server() { close(); }

		bool open(long port);
		bool isopen() { return _socket != INVALID_SOCKET; }

		bool wait(client &destination);

		void close();
	};
};

#elif defined __linux__

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include "custom/string.h"

namespace wsock
{
	namespace interface
	{
		class client
		{
		public:
			virtual bool open(string &source, long port) = 0;
			virtual bool isopen() = 0;

			virtual int read(char *buffer, int length, int flags) = 0;

			virtual int write(string &data, int flags) = 0;
			virtual int write(const char *buffer, int length, int flags) = 0;

			virtual void close() = 0;
		};

		class server
		{
		public:
			virtual bool open(long port) = 0;
			virtual bool isopen() = 0;
			virtual void close() = 0;
		};
	};

	class wsock
	{
	protected:
		const static long IPV4_LEN = 16L;
		const static long IPV6_LEN = 46L;

	public:
		wsock() { }
		~wsock() { }

		struct addrinfo *getHostByName(string &server);
	};

	class client : public wsock, public interface::client
	{
		const static int timeout;

	private:
		friend class server;

	protected:
		int _socket;

	public:
		client() : wsock() { _socket = -1; }

		bool open(string &source, long port);
		bool isopen() { return _socket != -1; }

		int read(char *buffer, int length, int flags);

		int write(string &data, int flags);
		int write(const char *buffer, int length, int flags);

		void close();
	};

	class server : public wsock, public interface::server
	{
	protected:
		int _socket;
		const static int timeout;

	public:
		server() : wsock() { _socket = -1; }
		~server() { close(); }

		bool open(long port);
		bool isopen() { return _socket != -1; }

		bool wait(client &destination);

		void close();
	};
};

#endif

#endif