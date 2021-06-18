#include <array>
#include <atomic>
//#include "data.h"
#include "string.h"

#ifndef _FIFO
#define _FIFO

template <class X, long Y> class fifo //: public data::fifo::bidirectional<X>
{
	bool init;

	std::array<X, Y> items;
	std::atomic<int> lpwrite;
	std::atomic<int> lpread;

public:
	fifo() { makeNull(); reset(); }
	~fifo() { cleanup(); }
	bool initalised() { return init; }
	void reset();

	unsigned long entries()
	{
		int r = lpread.load(std::memory_order_relaxed);
		int w = lpwrite.load(std::memory_order_relaxed);

		if (w < r) return (Y - r) + w;

		return w - r;
	}

	bool isfull()
	{
		int r = lpread.load(std::memory_order_relaxed);
		int w = lpwrite.load(std::memory_order_relaxed);

		return (w - 1) == r;
	}

	bool isempty()
	{
		int r = lpread.load(std::memory_order_relaxed);
		int w = lpwrite.load(std::memory_order_relaxed);

		return (r == w);
	}

	long size() { return Y; }

	void empty()
	{
		do
		{
			lpwrite.store(0);
			lpread.store(0);
		} while ((lpwrite != 0) || (lpread != 0));
	}

	bool get(X &destination);
	bool set(X &source);

	string identifier() { return string("fifo"); }

private:
	void cleanup() { }
	void makeNull() { }
};

template <class X, long Y> void fifo<X, Y>::reset()
{
	init = false; cleanup();

	empty();

	init = true;
}

template <class X, long Y> inline bool fifo<X, Y>::get(X &destination)
{
	if(lpread == lpwrite) return false;

	int a = lpread.load();
	int b = a + 1;
	if (b >= Y) b = 0;

	if (lpread.compare_exchange_weak(a, b, std::memory_order_release, std::memory_order_relaxed))
	{
		destination = X(items[a]);
		return true;
	}

	return false;
}


template <class X, long Y> inline bool fifo<X, Y>::set(X &source)
{
	int a = lpwrite.load();
	items[a] = source;
	int b = a + 1;
	if (b >= Y) b = 0;
	if (b == lpread) return false;

	return lpwrite.compare_exchange_weak(a, b, std::memory_order_release, std::memory_order_relaxed);
}

#endif