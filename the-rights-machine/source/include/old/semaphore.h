#include <atomic>

#ifndef _SEMAPHORE
#define _SEMAPHORE

class semaphore
{
public:
	class token
	{
	public:
		std::atomic<int> lock;

	public:
		token() { lock.store(0, std::memory_order_release); }
		token(const token &source) { }

		bool obtain()
		{
			int expected = 0;
			return lock.compare_exchange_weak(expected, 1, std::memory_order_release, std::memory_order_relaxed);
		}

		bool release()
		{
			int expected = 1;
			return lock.compare_exchange_weak(expected, 0, std::memory_order_release, std::memory_order_relaxed);
		}
	};

protected:
	token *value;

public:

	semaphore(token &source)
	{
		value = &source;
		while (!value->obtain()) {}
	}

	~semaphore()
	{
		while (!value->release()) {}
	}
};

#endif