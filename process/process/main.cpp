#include <Windows.h>

int APIENTRY WinMain(HINSTANCE hInstance,
					 HINSTANCE hPrevInstance,
					 LPSTR     lpCmdLine,
					 int       nCmdShow)
{

	// pass in database connection stirng
	// gpu ID
	// whether it exits after startup
	// or quits

	// create interface for C++ OR Cuda processing
	// template class for this, process, T = interface
	// process loads data from database tables

	// also, use template for database type, ODBC, mySQL



	// need lock for obtaining messages..????

	// get message, update "process status" in table

	// or just assume one database per table, let the higher level deal with
	// a round robin approach

	// status : 1 = new, 2 = processing, 3 = done, 4 = error

	// only need to compute overlapping results
	// split lines into multiple lines!!!!   in xxxxxxxx
	//                                   out     xxxx
	// results in three "has" positions
	return 0;
}