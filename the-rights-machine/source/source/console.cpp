#include "console.h"
#include "error.h"

#ifdef WIN32

void console::console::reset(int x, int y)
{
	init = false; cleanup();

	dimensions.X = columns;
	dimensions.Y = rows;

	position.X = 0;
	position.Y = 0;

	region.Top = 0;
	region.Left = 0;
	region.Right = columns - 1;
	region.Bottom = rows - 1;

	if (!AllocConsole())
	{
		setLastError<::console::console>(string("reset::AllocConsole"));

		return;
	}

	handle = GetStdHandle(STD_OUTPUT_HANDLE);
	if (handle == INVALID_HANDLE_VALUE)
	{
		setLastError<::console::console>(string("reset::GetStdHandle"));

		return;
	}

	if (GetConsoleScreenBufferInfo(handle, &info) == FALSE)
	{
		setLastError<::console::console>(string("reset::GetConsoleScreenBufferInfo"));

		return;
	}

	if (SetConsoleWindowInfo(handle, TRUE, &region) == FALSE)
	{
		setLastError<::console::console>(string("reset::SetConsoleWindowInfo"));

		return;
	}

	if (SetConsoleScreenBufferSize(handle, dimensions) == FALSE)
	{
		setLastError<::console::console>(string("reset::SetConsoleScreenBufferSize"));

		return;
	}

	if (!title("Console"))
	{
		setLastError<::console::console>(string("reset::title"));

		return;
	}

	if (!cursor(false))
	{
		setLastError<::console::console>(string("reset::cursor"));

		return;
	}

	if (!move(x, y))
	{
		setLastError<::console::console>(string("reset::move"));

		return;
	}

	clear();

	init = true;
}

void console::console::clear()
{
	memset(buffer, 0, sizeof(CHAR_INFO) * columns * rows);
}

bool console::console::move(int x, int y)
{
	HWND consoleWindow = GetConsoleWindow();

	return (SetWindowPos(consoleWindow, 0, x, y, 0, 0, SWP_NOSIZE | SWP_NOZORDER) == TRUE);
}

bool console::console::title(string source)
{
	return title(source.c_str());
}

void console::console::write(string source, bool timestamp)
{
	if (timestamp)
	{
		time_t now = time(NULL);
		string temp;
		temp.concat(string::fromTime(now));
		temp.concat("> ");
		temp.concat(source);

		write(temp.c_str(), temp.count());
	}
	else write(source.c_str(), source.count());
}

bool console::console::title(const char *source)
{
	return SetConsoleTitle(source) == TRUE;
}

void console::console::write(const char *source, int length)
{
	semaphore lock(token);

	int previous = (rows - 2)  * columns;
	int current = (rows - 1) * columns;

	for (int i = 0; i < rows - 1; ++i)
	{
		memcpy(&buffer[current], &buffer[previous], sizeof(CHAR_INFO) * columns);
		current = previous;
		previous -= columns;
	}

	memset(&buffer[0], 0, sizeof(CHAR_INFO) * columns);

	CHAR_INFO temp;
	temp.Attributes = '0\17';

	if (length > columns) length = columns;

	for (int i = 0; i < length; ++i)
	{
		temp.Char.AsciiChar = source[i];
		buffer[i] = temp;
	}

	refresh();
}

bool console::console::cursor(bool enable)
{
	CONSOLE_CURSOR_INFO cursorInfo;

	if (GetConsoleCursorInfo(handle, &cursorInfo) == FALSE) return false;
	cursorInfo.bVisible = enable == true ? TRUE : FALSE;
	if (SetConsoleCursorInfo(handle, &cursorInfo) == FALSE) return false;

	return true;
}

bool console::console::refresh()
{
	return (WriteConsoleOutput(handle, buffer, dimensions, position, &region) == TRUE);
}

void console::console::makeNull()
{
	handle = INVALID_HANDLE_VALUE;
}

void console::console::cleanup()
{
	FreeConsole();
}

#elif defined(__linux__)
#include <X11/Xatom.h>
#include <X11/Xutil.h>

void console::console::reset(int x, int y)
{
	init = false; cleanup();

	XInitThreads();

	display = XOpenDisplay(NULL);
	visual = DefaultVisual(display, 0);
	depth = DefaultDepth(display, 0);

	int width = 6 * columns;
	int height = 13 * rows;

	XSetWindowAttributes frame_attributes;
	frame_attributes.background_pixel = XBlackPixel(display, 0);
	window = XCreateWindow(display, XRootWindow(display, 0),
								x, y, width, height, 5, depth,
								InputOutput, visual, CWBackPixel, &frame_attributes);

	XSelectInput(display, window, ExposureMask | StructureNotifyMask);

	fix(width, height);

	fontinfo = XLoadQueryFont(display, "6x13");
	if (fontinfo == NULL)
	{
		//setLastError<::console::console>(string("reset::XLoadQueryFont(6x13)"));

		return;
	}

	gr_values.font = fontinfo->fid;
	gr_values.foreground = XWhitePixel(display, 0);
	graphical_context = XCreateGC(display, window, GCFont + GCForeground, &gr_values);
	XMapWindow(display, window);

	if (!title("Console"))
	{
		//setLastError<::console::console>(string("reset::title"));

		return;
	}

	if (!move(x, y))
	{
		//setLastError<::console::console>(string("reset::move"));

		return;
	}

	clear();

	init = true;
}

void console::console::clear()
{
	#warning got lazy
	//memset(buffer, 0, columns * rows);
}

bool console::console::move(int x, int y)
{
	return true;
}

bool console::console::title(string source)
{
	return title(source.c_str());
}

void console::console::write(string source, bool timestamp)
{
	if (timestamp)
	{
		time_t now = time(NULL);
		string temp;
		temp.concat(string::fromTime(now));
		temp.concat(string("> "));
		temp.concat(source);

		write(temp.c_str(), temp.count());
	}
	else write(source.c_str(), source.count());
}

bool console::console::title(const char *source)
{
	XStoreName(display, window, source);

	return true;
}

void console::console::write(const char *source, int length)
{
	/*
	semaphore lock(token);

	int previous = (rows - 2)  * columns;
	int current = (rows - 1) * columns;

	for (int i = 0; i < rows - 1; ++i)
	{
		memcpy(&buffer[current], &buffer[previous], columns);
		current = previous;
		previous -= columns;
	}

	memset(&buffer[0], 0, columns);

	if (length > columns) length = columns;

	for (int i = 0; i < length; ++i)
	{
		buffer[i] = source[i];
	}

	refresh();
	*/
}

bool console::console::refresh()
{
	#warning got lazy
	// it's the pstring thing, the file named string.h 
	/*
	XWindowAttributes window_attributes;
	int font_direction, font_ascent, font_descent;
	XCharStruct text_structure;

	XClearWindow(display, window);

	int offset = 0, x = 2, y = 12;
	for (int row = 0; row < rows; ++row)
	{
		char temp[columns];
		memset(temp, 0, columns);
		memcpy(temp, &buffer[offset], columns);

		XTextExtents(fontinfo, temp, strlen(temp), &font_direction, &font_ascent, &font_descent, &text_structure);
		XGetWindowAttributes(display, window, &window_attributes);
		XDrawString(display, window, graphical_context, x, y, temp, strlen(temp));

		y += text_structure.ascent + text_structure.descent + 2;
		offset += columns;
	}
*/
	return true;
}

void console::console::fix(int w, int h)
{
	XSizeHints *sh = XAllocSizeHints();
	sh->flags = PMinSize | PMaxSize;
	sh->min_width = sh->max_width = 100;
	sh->min_height = sh->max_height = 100;
	XSetWMSizeHints(display, window, sh, XA_WM_SIZE_HINTS);
	XFree(sh);
}

void console::console::makeNull()
{
	display = NULL;
	visual = NULL;
}

void console::console::cleanup()
{
	if (display != NULL)
	{
		XDestroyWindow(display, window);
		XCloseDisplay(display);
	}
}

#endif