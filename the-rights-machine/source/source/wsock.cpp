#include "wsock.h"
#include "log.h"
#include <memory.h>
#include <cstring>

#ifdef WIN32

#include <ws2tcpip.h>

int wsock::wsock::constructed = 0;
bool wsock::wsock::init = false;

wsock::wsock::wsock()
{
	WORD sockVersion,result;
	WSADATA wsaData;

	if(constructed==0)
	{
		sockVersion = MAKEWORD(2,0);
		result = WSAStartup(sockVersion,&wsaData);

		if (result == 0) init = true;
	}

	++constructed;
};

wsock::wsock::~wsock()
{
	--constructed;
	if ((constructed <= -1) && (init == true)) { WSACleanup(); init = false; }
}

struct addrinfo *wsock::wsock::getHostByName(string &server)
{
	struct addrinfo *result = NULL;

	if (getaddrinfo(server.c_str(), NULL, NULL, &result) != 0) result = NULL;

	return result;
}

bool wsock::client::open(string &server, long port)
{
	struct addrinfo *host;
	SOCKADDR_IN details;

	host = getHostByName(server);
	if(host==NULL) return false;

	_socket = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	if (_socket == INVALID_SOCKET) return false;

	char buffer[IPV4_LEN];
	memset(buffer, 0, IPV4_LEN);
	const char *ip = inet_ntop(AF_INET, &(((SOCKADDR_IN*)host->ai_addr)->sin_addr), buffer, sizeof(buffer));
	if (ip == NULL) return false;

	memset((char*)&details,0,sizeof(struct sockaddr));
	details.sin_family = AF_INET;
	if (inet_pton(AF_INET, ip, &(details.sin_addr)) != 1) return false;

	details.sin_port = htons((u_short)port);
	int result = connect(_socket, (LPSOCKADDR)&details, sizeof(struct sockaddr));
	if(result == SOCKET_ERROR) return false;

	result = setsockopt(_socket, SOL_SOCKET, SO_RCVTIMEO, (char *)&client::timeout, sizeof(int));
	if (result == SOCKET_ERROR) return false;

	result = setsockopt(_socket, SOL_SOCKET, SO_SNDTIMEO, (char *)&client::timeout, sizeof(int));
	if (result == SOCKET_ERROR) return false;

	return true;
}

int wsock::client::read(char *buffer, int length, int flags)
{
	return recv(_socket,buffer,length, flags);
}

int wsock::client::write(string &data, int flags)
{
	int result = write(data.c_str(), (int)data.count(), flags);

	return result;
}

int wsock::client::write(const char *buffer, int length, int flags)
{
	int remaining = length;
	int offset = 0;

	while (remaining > 0)
	{
		int sent = send(_socket, &buffer[offset], remaining, flags);
		if (sent == SOCKET_ERROR) return SOCKET_ERROR;
		remaining -= sent; offset += sent;
	}

	return length;
}

void wsock::client::close()
{
	shutdown(_socket, SD_BOTH);
	closesocket(_socket);

	_socket = INVALID_SOCKET;
}


bool wsock::server::open(long port)
{
	SOCKADDR_IN details;

	_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (_socket == INVALID_SOCKET) return false;

	memset((char*)&details, 0, sizeof(struct sockaddr));
	details.sin_family = AF_INET;
	details.sin_addr.s_addr = INADDR_ANY;
	details.sin_port = htons((u_short)port);

	if (bind(_socket, (LPSOCKADDR)&details, sizeof(struct sockaddr)) == SOCKET_ERROR) return false;

	if (listen(_socket, 1) == SOCKET_ERROR) return false;

	int result = setsockopt(_socket, SOL_SOCKET, SO_RCVTIMEO, (char *)&server::timeout, sizeof(int));
	if (result == SOCKET_ERROR) return false;

	result = setsockopt(_socket, SOL_SOCKET, SO_SNDTIMEO, (char *)&server::timeout, sizeof(int));
	if (result == SOCKET_ERROR) return false;

	return true;
}

bool wsock::server::wait(client &destination)
{
	if (_socket != NULL)
	{
		SOCKET result = accept(_socket, NULL, NULL);
		if (result != INVALID_SOCKET)
		{
			destination._socket = result;

			return true;
		}
	}

	return false;
}

void wsock::server::close()
{
	shutdown(_socket, SD_BOTH);
	closesocket(_socket);

	_socket = INVALID_SOCKET;
}

#elif defined(__linux__)

const int wsock::client::timeout = 5;
const int wsock::server::timeout = 5;

struct addrinfo *wsock::wsock::getHostByName(string &server)
{
	struct addrinfo *result = NULL;

	if (getaddrinfo(server.c_str(), NULL, NULL, &result) != 0) result = NULL;

	return result;
}

bool wsock::client::open(string &server, long port)
{
	struct addrinfo *host;
	sockaddr_in details;

	host = getHostByName(server);
	if (host == NULL) return false;

	_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (_socket == -1) return false;

	char buffer[IPV4_LEN];
	memset(buffer, 0, IPV4_LEN);
	const char *ip = inet_ntop(AF_INET, &(((sockaddr_in*)host->ai_addr)->sin_addr), buffer, sizeof(buffer));
	if (ip == NULL) return false;

	memset((char*)&details, 0, sizeof(struct sockaddr));
	details.sin_family = AF_INET;
	if (inet_pton(AF_INET, ip, &(details.sin_addr)) != 1) return false;

	details.sin_port = htons((u_short)port);
	int result = connect(_socket, (struct sockaddr*)&details, sizeof(struct sockaddr));
	if (result < 0) return false;

	struct timeval tv = { client::timeout, 0 };

	result = setsockopt(_socket, SOL_SOCKET, SO_RCVTIMEO, (struct timeval *)&tv, sizeof(struct timeval));
	if (result < 0) return false;

	result = setsockopt(_socket, SOL_SOCKET, SO_SNDTIMEO, (struct timeval *)&tv, sizeof(struct timeval));
	if (result < 0) return false;

	return true;
}

int wsock::client::read(char *buffer, int length, int flags)
{
	return recv(_socket, buffer, length, flags);
}

int wsock::client::write(string &data, int flags)
{
	int result = write(data.c_str(), (int)data.count(), flags);

	return result;
}

int wsock::client::write(const char *buffer, int length, int flags)
{
	int remaining = length;
	int offset = 0;

	while (remaining > 0)
	{
		int sent = send(_socket, &buffer[offset], remaining, flags | MSG_NOSIGNAL);
		if (sent == -1) return -1;
		remaining -= sent; offset += sent;
	}

	return length;
}

void wsock::client::close()
{
	shutdown(_socket, SHUT_RDWR);
	::close(_socket);
	_socket = -1;
}

bool wsock::server::open(long port)
{
	sockaddr_in details;

	_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (_socket == -1) return false;

	memset((char*)&details, 0, sizeof(struct sockaddr));
	details.sin_family = AF_INET;
	details.sin_addr.s_addr = INADDR_ANY;
	details.sin_port = htons((u_short)port);

	if (bind(_socket, (struct sockaddr*)&details, sizeof(struct sockaddr)) < 0) return false;

	if (listen(_socket, 1) < 0) return false;

	struct timeval tv = { server::timeout, 0 };

	int result = setsockopt(_socket, SOL_SOCKET, SO_RCVTIMEO, (struct timeval *)&tv, sizeof(struct timeval));
	if (result < 0) return false;

	result = setsockopt(_socket, SOL_SOCKET, SO_SNDTIMEO, (struct timeval *)&tv, sizeof(struct timeval));
	if (result < 0) return false;

	return true;
}

bool wsock::server::wait(client &destination)
{
	if (_socket != -1)
	{
		int result = accept(_socket, NULL, NULL);
		if (result != -1)
		{
			destination._socket = result;

			return true;
		}
	}

	return false;
}

void wsock::server::close()
{
	shutdown(_socket, SHUT_RDWR);
	::close(_socket);
	_socket = -1;
}

#endif