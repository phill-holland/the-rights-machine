#include "thread.h"

#if !defined(__LIST)
#define __LIST

// #warning delete me

namespace custom
{
	template <class X> class list
	{
		class buffer
		{
		public:
			X data;
			buffer *next;

		public:
			buffer() { next = NULL; }
			~buffer() { }
		};

	protected:
		buffer * data, *last;

		buffer *last_indexed_ptr;
		long last_indexed;

		long elements;

		mutex::token token;

		bool init;

	public:
		list() { makeNull(); reset(); }
		~list() { cleanup(data); }

		bool initalised() { return init; }

		void reset()
		{
			mutex lock(token);

			init = false; cleanup(data);
			elements = 0L;

			data = new buffer();
			if (data == NULL) return;

			last = data;

			last_indexed_ptr = NULL;
			last_indexed = -1L;

			init = true;
		}

		unsigned long count() { return elements; }

		bool isempty() { if (elements <= 0L) return true; return false; }

		void empty() { reset(); }

		bool add(X item)
		{
			mutex lock(token);

			bool result = false;

			last->data = item;
			last->next = new buffer();
			if (last->next != NULL)
			{
				last = last->next;
				++elements;

				result = true;
			}

			return result;
		}

		X remove(long index)
		{
			if (index >= elements) return X();

			mutex lock(token);

			X result = X();

			last_indexed_ptr = NULL;
			last_indexed = -1L;

			bool found = false;
			long offset = 0L;
			buffer *current = data, *previous = NULL;

			do
			{
				if (offset == index)
				{
					if (previous != NULL)
					{
						previous->next = current->next;
						result = current->data;
						delete current;
					}
					else
					{
						data = current->next;
						result = current->data;
						delete current;
					}

					--elements;
					found = true;
				}
				previous = current;
				current = current->next;
				++offset;
			} while ((offset <= (long)index) && (!found));

			return result;
		}

		X operator[](int index)
		{
			if (index >= elements) return X();

			mutex lock(token);

			X result = X();

			bool found = false;
			long offset = 0L;
			buffer *current = data;

			if ((index - 1L == last_indexed) && (last_indexed_ptr != NULL))
			{
				offset = last_indexed;
				current = last_indexed_ptr;
			}
			else
			{
				last_indexed_ptr = NULL;
				last_indexed = -1L;
			}

			do
			{
				if (offset == index)
				{
					last_indexed_ptr = current;
					last_indexed = index;

					result = current->data;
					found = true;
				}
				current = current->next;
				++offset;
			} while ((offset <= (long)index) && (!found));

			return result;
		}

	protected:
		void makeNull() { data = NULL; }

		void cleanup(buffer *source)
		{
			if (source != NULL)
			{
				if (source->next != NULL)
				{
					cleanup(source->next);
				}

				delete source;
			}
		}
	};
};

#endif
