#include "error.h"
#include "thread.h"
#include "queue.h"
#include "console.h"
#include <vector>

#if !defined(__ERRORS)
#define __ERRORS

namespace error
{
	// have option, output to console, or database
	// both respond to queue (or log class!!)

	class errors : public thread, public queue::in<::error::error>//, public allocator::allocator<::error::error, 10L>
	{
		static const long EXPIRATION = 20L;
		static const long THRESHOLD = 10L;

	private:
		queue::in<::error::error> *destination;
		std::vector<::error::error> queue;
		
		mutex::token token;

		long counter;
		
		bool init;

	public:
		DWORD WINAPI background(thread *bt);

	public:
		errors(::queue::in<::error::error> *destination) { makeNull(); reset(destination); }
		~errors() { cleanup(); }

		bool initalised() { return init; }
		void reset(::queue::in<::error::error> *destination);

		void clear();

		bool set(::error::error &source);

	protected:
		bool flush();

	protected:
		void makeNull();
		void cleanup();
	};

	namespace console
	{
		class errors : public queue::in<::error::error>, public ::console::console
		{
		public:
			bool set(::error::error &source)
			{
				return console::set((string)source);
			}
		};
	};
};

#endif