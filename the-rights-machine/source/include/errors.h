#include "error.h"
#include "thread.h"
#include "queue.h"

#if !defined(__ERRORS)
#define __ERRORS

namespace error
{
	class errors : public thread, public ::error::type::types, public queue::in<::error::error>
	{
		static const long EXPIRATION = 20L;
		static const long THRESHOLD = 10L;
		static const long MAX = 20L;

	private:
		queue::in<::error::type::type> *destination;

		::error::error **data;
		long length;

		mutex::token token;

		long counter;

		bool init;

	public:
		void background(thread *bt);

	public:
		errors(::queue::in<::error::type::type> *destination) { makeNull(); reset(destination); }
		~errors() { cleanup(); }

		bool initalised() { return init; }
		void reset(::queue::in<::error::type::type> *destination);

		void clear();

		bool set(::error::error &source);

		bool shutdown() { return stopAndWait(); }

	protected:
		bool flush();
		void reset();

	protected:
		void makeNull();
		void cleanup();
	};
};

#endif