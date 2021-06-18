#include <mutex>

#ifndef _MUTEX
#define _MUTEX

#ifndef _MUTEX_WINDOWS

class mutex : public std::lock_guard<std::mutex>
{
public:
	class token : public std::mutex
	{
	public:
		token() { }
		token(const token &source) { }
	};

public:
	mutex(token &t) : std::lock_guard<std::mutex>(t) { }
};

#else

#include <windows.h>

class mutex
{
public:
	class token
	{
	public:
		CRITICAL_SECTION critical;

	public:
		token() { InitializeCriticalSection(&critical); }
		~token() { DeleteCriticalSection(&critical); }
	};

protected:
	token *temp;

public:
	mutex(token &t)
	{
		temp = &t;
		EnterCriticalSection(&t.critical);
	}

	~mutex() { LeaveCriticalSection(&temp->critical); }
};

#endif

#endif