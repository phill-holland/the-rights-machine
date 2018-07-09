#include "queue.h"

#if !defined(__ALLOCATOR)
#define __ALLOCATOR

namespace allocator
{

	template <class X, long Y> class allocator : public queue::queue<X>
	{
		template <class X, long Y> class block
		{
		public:
			block *next, *previous;
			X **data;

		private:					
			long write, read;

			bool init;

		public:
			block() { makeNull(); reset(); }
			block(block const &source) { clear(); copy(source); }
			~block() { cleanup(); }

			bool initalised() { return init; }

			void reset()
			{
				init = false; cleanup();

				write = 0L; read = 0L;
				next = NULL; previous = NULL;

				data = new X*[Y];
				for (long i = 0L; i < Y; ++i) data[i] = NULL;

				for (long i = 0L; i < Y; ++i)
				{
					data[i] = new X();
					if (data[i] == NULL) return;
				}

				init = true;
			}

			bool isfull()
			{
				return (write >= Y);
			}

			bool isempty()
			{
				return (read == write);
			}

			bool set(X &source)
			{
				if (write >= Y) return false;
				*data[write++] = source;

				return true;
			}

			bool get(X &destination)
			{
				if (read >= Y) return false;
				destination = *data[read++];

				return true;
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

					read = 0L;
					write = source.write;
				}
			}

		public:
			block& operator=(const block& source)
			{
				this->copy((block&)source);
				return *this;
			}

		protected:
			void makeNull()
			{
				data = NULL;
			}

			void cleanup()
			{
				if (data != NULL)
				{
					for (long i = Y - 1L; i >= 0L; i--)
					{
						if (data[i] != NULL) delete data[i];
					}

					delete data;
				}
			}
		};

		block<X, Y> *head, *tail, *read;

		long elements;
		long counter;

		bool init;

	public:
		allocator() { makeNull(); reset(); }
		allocator(allocator const &source) { clear(); copy(source); }
		~allocator() { cleanup(); }

		bool initalised() { return init; }

		void reset()
		{
			init = false; cleanup();

			head = NULL; tail = NULL; read = NULL;
			elements = 0L;

			init = true;
		}

		virtual void clear() { }

		bool get(X &destination)
		{
			if (read == NULL) return false;

			if ((read->isempty()) && (read->isfull())) read = read->next;
			if (read == NULL) return false;

			return read->get(destination);
		}

		bool set(X &source)
		{
			if (head == NULL)
			{
				head = new block<X, Y>();
				if (head == NULL) return false;
				if (!head->initalised()) return false;

				tail = head; read = head;
			}

			if (tail->isfull())
			{
				tail->next = new block<X, Y>();
				if (tail->next == NULL) return false;
				if (!tail->next->initalised()) return false;

				tail->next->previous = tail;				
				tail = tail->next;

				if (!tail->set(source)) return false;
			}
			
			++elements;

			return true;
		}

		virtual bool flush() { return true; }

		bool copy(allocator const &source)
		{
			reset();

			block<X, Y> *src = source.head;
			while (src != NULL)
			{
				for (long i = 0L; i < Y; ++i)
				{
					set(*src->data[i]);
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

	protected:
		void makeNull()
		{
			head = NULL;
			tail = NULL;
		}

		void cleanup()
		{
			while (tail != NULL)
			{
				block<X, Y> *temp = tail->previous;
				delete tail;
				tail = temp;
			}				
		}
	};
};

#endif