#include "queue.h"
#include <stdio.h>
#include <stdlib.h>

#if !defined(__ALLOCATOR)
#define __ALLOCATOR

namespace allocator
{
	// THIS NEEDS MUTEX LOCKS
	template <class X, long Y> class block
	{
	public:
		block *next, *previous;
		X **data;

	private:
		long write;// , read;

		bool init;

	public:
		block() { makeNull(); reset(); }
		block(block const &source) { clear(); copy(source); }
		~block() { cleanup(); }

		bool initalised() { return init; }

		void reset()
		{
			init = false; cleanup();

			write = 0L; //read = 0L;
			next = nullptr; previous = nullptr;

			data = new X*[Y];
			for (long i = 0L; i < Y; ++i) data[i] = nullptr;

			for (long i = 0L; i < Y; ++i)
			{
				data[i] = new X();
				if (data[i] == nullptr) return;
			}

			init = true;
		}

		void clear() 
		{ 
			write = 0L;
			next = nullptr; previous = nullptr;
		}

		long count() { return write; }

		bool isfull()
		{
			return (write >= Y);
		}

		//bool isempty()
		//{
		//	return (read == write);
		//}

		bool set(X &source)
		{
			if (write >= Y) return false;
			*data[write++] = source;

			return true;
		}

		//bool get(X &destination) { return false; }
		/*{
			if (read >= Y) return false;
			destination = *data[read++];

			return true;
		}*/

		X get(long index)
		{
			if ((index < 0) || (index >= Y)) return X();
			return *data[index];
		}

		virtual bool flush() { return true; }

		void copy(block const &source)
		{
			if ((initalised()) && (source.initialised()))
			{
				for (long i = 0L; i < Y; ++i)
				{
					*data[i] = *(source.data[i]);
				}

				//read = 0L;
				write = source.write;
			}
		}

	public:
		block& operator=(const block& source)
		{
			this->copy((block&)source);
			return *this;
		}

		X operator[](int index)
		{
			return get((long)index);
		}

	protected:
		void makeNull()
		{
			data = nullptr;
		}

		void cleanup()
		{
			if (data != nullptr)
			{
				for (long i = Y - 1L; i >= 0L; i--)
				{
					if (data[i] != nullptr) delete data[i];
				}

				delete data;
			}
		}
	};

	template <class X, long Y> class cursor
	{
	public:
		block<X, Y> *current;
		long index, normalised;

	public:
		cursor()
		{
			current = nullptr;
			index = 0;
			normalised = 0;
		}
	};

	template <class X, long Y> class allocator : public queue::in<X>, public queue::base//queue<X>
	{
		block<X, Y> *head, *tail;// , *read;

		cursor<X, Y> accessor;

		long elements;
		long counter;

		bool init;

	public:
		allocator() { makeNull(); reset(); }
		allocator(allocator const &source) { clear(); copy(source); }
		~allocator() { cleanup(); }

		bool initalised() { return init; }

		void clear()
		{
			head = nullptr; tail = nullptr;// read = nullptr;
			elements = 0L;
		}

		void reset()
		{
			init = false; cleanup();

			head = nullptr; tail = nullptr;// read = nullptr;
			elements = 0L;

			init = true;
		}

		//virtual void clear() { }

		//bool get(X &destination) { return false; }
		/*
		{
			if (read == nullptr) return false;

			if ((read->isempty()) && (read->isfull())) read = read->next;
			if (read == nullptr) return false;

			return read->get(destination);
		}
		*/
		long count() { return elements; }

		X get(long index)
		{
			if ((index < 0L) || (index >= elements)) return X();

			if ((accessor.current == nullptr) || (accessor.index + 1L != index))
			{
				//if ((index >= 0) && (index < elements))
				//{
					ldiv_t t = div((long)index, Y);

					int counter = 0;
					block<X, Y> *src = head;
					while ((src != nullptr) && (counter < t.quot))
					{
						src = src->next;
						++counter;
					};

					if (src != nullptr)
					{
						accessor.index = t.quot + t.rem;
						accessor.normalised = t.rem;
						accessor.current = src;
						return (*src)[t.rem];
					}
				//}
			}
			else
			{
				if (accessor.index + 1L == index)
				{
					++accessor.index;
					++accessor.normalised;
					if (accessor.normalised >= Y)
					{
						accessor.normalised = 0;
						accessor.current = accessor.current->next;
					}

					if (accessor.current != nullptr)
					{
						//block<X, Y> *c = accessor.current;
						return (*accessor.current)[accessor.normalised];
						//return accessor.((*current)[accessor.normalised]);
					}
				}
			}

			return X();
		}

		bool set(X &source)
		{
			if (head == nullptr)
			{
				head = new block<X, Y>();
				if (head == nullptr) return false;
				if (!head->initalised()) return false;

				tail = head; //read = head;
			}

			if (tail->isfull())
			{
				tail->next = new block<X, Y>();
				if (tail->next == nullptr) return false;
				if (!tail->next->initalised()) return false;

				tail->next->previous = tail;
				tail = tail->next;
			}

			if (!tail->set(source)) return false;

			++elements;

			return true;
		}

		//virtual bool flush() { return true; }

		bool copy(allocator const &source)
		{
			reset();

			block<X, Y> *src = source.head;
			while (src != nullptr)
			{
				for (long i = 0L; i < src->count(); ++i)
				{
					if (!set(*src->data[i])) return false;
				}
				src = src->next;
			}

			return true;
		}

	public:
		allocator& operator=(const allocator& source)
		{
			this->copy((allocator&)source);
			return *this;
		}

		X operator[](int index)
		{
			return get((long)index);
		}

	protected:
		void makeNull()
		{
			head = nullptr;
			tail = nullptr;
		}

		void cleanup()
		{
			while (tail != nullptr)
			{
				block<X, Y> *temp = tail->previous;
				delete tail;
				tail = temp;
			}
		}
	};
};

#endif