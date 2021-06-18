#ifndef _WINCONSOLE
#define _WINCONSOLE

#ifdef WIN32

#include <windows.h>
#include "data.h"
#include "pstring.h"
#include "thread.h"
#include "semaphore.h"

namespace console
{
	class console : public data::fifo::destination<string>
	{
		CONSOLE_SCREEN_BUFFER_INFO info;

		COORD dimensions, position;
		SMALL_RECT region;

		const static SHORT columns = 80, rows = 25;
		CHAR_INFO buffer[columns * rows];

		HANDLE handle;

		semaphore::token token;

		bool init;

	public:
		console(int x = 0, int y = 0) { makeNull(); reset(x, y); }
		~console() { cleanup(); }

		void reset(int x, int y);
		bool initalised() { return init; }

		void clear();
		bool move(int x, int y);

		bool title(string source);
		void write(string source, bool timestamp = true);

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

#elif defined __linux__

//#include "data.h"
#include "custom/string.h"
#include "thread.h"
#include "semaphore.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <X11/Xlib.h>

namespace console
{
	class console //: public data::fifo::destination<string>
	{
		bool init;

		Display *display;
		Visual *visual;
		int depth;
		Window window;
		XFontStruct *fontinfo;
		XGCValues gr_values;
		GC graphical_context;

		const static int columns = 80, rows = 25;
		char buffer[columns * rows];

		semaphore::token token;

	public:
		console(int x = 0, int y = 0) { makeNull(); reset(x, y); }
		~console() { cleanup(); }

		void reset(int x, int y);
		bool initalised() { return init; }

		void clear();
		bool move(int x, int y);

		bool title(string source);
		void write(string source, bool timestamp = true);

		bool set(string &source) { write(source); return true; }
		bool isfull() { return false; }

		string identifier() { return string("console"); }

	protected:
		bool title(const char *source);
		void write(const char *source, int length);

	protected:
		bool refresh();
		void fix(int w, int h);

	public:
		console operator<<(string &source) { write(source); return *this; }

	protected:
		void makeNull();
		void cleanup();
	};
};

#endif

#endif