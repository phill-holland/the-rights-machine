#include "console.h" 

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

	if (!AllocConsole()) return;

	handle = GetStdHandle(STD_OUTPUT_HANDLE);
	if (handle == INVALID_HANDLE_VALUE) return;

	if (GetConsoleScreenBufferInfo(handle, &info) == FALSE) return;
	if (SetConsoleWindowInfo(handle, TRUE, &region) == FALSE) return;
	if (SetConsoleScreenBufferSize(handle, dimensions) == FALSE) return;

	if (!title("Console")) return;
	if (!cursor(false)) return;
	if (!move(x, y)) return;

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

bool console::console::title(string &source)
{
	return title(source.c_str());
}

void console::console::write(string &source, bool timestamp)
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
	mutex lock(token);

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
