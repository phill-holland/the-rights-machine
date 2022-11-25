#include "core/string/string.h"

#if !defined(__CHARBUF)
#define __CHARBUF

namespace global
{
	class charbuf
	{
		static const unsigned long LENGTH = 255UL;

		unsigned long length, items;
		char *buffer;
		
		bool init;

	public:
		charbuf(unsigned long length = LENGTH) { makeNull(); reset(length); }
		charbuf(charbuf const &source) { makeNull(); copy(source); }
		~charbuf() { cleanup(); }

		bool initalised() { return init; }
		void reset(unsigned long length);

		void clear();

		bool isempty() { return items == 0UL; }

		bool push(char value);
		string get() { return string(buffer); }

		unsigned long count() { return items; }

		bool compare(string value);

		void copy(charbuf const &source);

	public:
		operator string() { return get(); }
		charbuf operator=(const charbuf& source)
		{
			this->copy((charbuf&)source);
			return *this;
		}

	protected:
		void makeNull();
		void cleanup();
	};
};

#endif