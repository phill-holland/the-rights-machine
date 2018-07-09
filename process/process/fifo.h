#include <array>
#include "thread.h"
#include "string.h"
#include "queue.h"

#if !defined(__FIFO)
#define __FIFO

namespace custom
{
	template <class X, long Y> class fifo : public queue::queue<X>
	{
		bool init;

		mutex::token token;

		std::array<X, Y> items;
		long lpread, lpwrite, elements;

	public:
		fifo() { makeNull(); reset(); }
		fifo(fifo const &source) { makeNull(); reset(); copy(source); }
		~fifo() { cleanup(); }

		bool initalised() { return init; }
		void reset();

		unsigned long entries() { mutex lock(token); return elements; }

		bool isfull() { mutex(token); return _isfull(); }
		bool isempty() { mutex(token); return _isempty(); }

		long size() { return Y; }

		void empty()
		{
			mutex lock(token);

			elements = 0L; lpread = 0L; lpwrite = 0L;
		}

		bool get(X &destination);
		bool set(X &source);

		virtual bool flush() { return true; }

		string identifier() { return string("fifo"); }

		void copy(fifo const &source)
		{
			mutex a_lock(token);
			mutex b_lock((mutex::token&)source.token);

			elements = source.elements;
			lpread = source.lpread;
			lpwrite = source.lpwrite;

			items = source.items;
		}

	public:
		fifo& operator=(const fifo &source)
		{
			this->copy((fifo&)source);
			return *this;
		}

	protected:
		bool _isfull() { if (elements >= Y - 1L) return true; return false; }
		bool _isempty() { if (elements <= 0L) return true;  return false; }

	protected:
		long inc(long src)
		{
			long temp = src;
			++temp;
			if (temp >= Y) temp = 0L;
			return temp;
		}

	private:
		void cleanup() { }
		void makeNull() { }
	};

	template <class X, long Y> void fifo<X, Y>::reset()
	{
		mutex lock(token);

		init = false; cleanup();

		lpread = 0L;
		lpwrite = 0L;
		elements = 0L;

		init = true;
	}

	template <class X, long Y> inline bool fifo<X, Y>::get(X &destination)
	{
		bool result = false;

		mutex lock(token);

		if (!_isempty())
		{
			--elements;

			destination = X(items[lpread]);
			lpread = inc(lpread);
			result = true;
		}

		return result;
	}

	template <class X, long Y> inline bool fifo<X, Y>::set(X &source)
	{
		bool result = false;

		mutex lock(token);

		if (!_isfull())
		{

			++elements;
			items[lpwrite] = source;
			lpwrite = inc(lpwrite);

			result = true;
		}

		return result;
	}
};

#endif
