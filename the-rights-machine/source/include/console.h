#include <windows.h>
//#include "data.h"
#include "string.h"
#include "thread.h"
#include "queue.h"

#if !defined(__CONSOLE)
#define __CONSOLE

namespace console
{
	class console : public queue::in<string>//: public data::fifo::destination<string>
	{
		CONSOLE_SCREEN_BUFFER_INFO info;

		COORD dimensions, position;
		SMALL_RECT region;

		const static SHORT columns = 80, rows = 25;
		CHAR_INFO buffer[columns * rows];

		HANDLE handle;

		mutex::token token;

		bool init;

	public:
		console(int x = 0, int y = 0) { makeNull(); reset(x, y); }
		~console() { cleanup(); }

		void reset(int x, int y);
		bool initalised() { return init; }

		void clear();
		bool move(int x, int y);

		bool title(string &source);
		void write(string &source, bool timestamp = true);

		bool set(string &source) { write(source); return true; }
		bool isfull() { return false; }

		string identifier() { return string("console"); }

	protected:
		bool title(const char *source);
		void write(const char *source, int length);

	public:
		console operator<<(string &source) { write(source); return *this; }

	protected:
		bool refresh();
		bool cursor(bool enable);

	protected:
		void makeNull();
		void cleanup();
	};
};

#endif