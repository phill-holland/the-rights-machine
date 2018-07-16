#include "wsock.h"
#include <windows.h>
#include "configuration.h"
#include "manager.h"
#include "queues.h"
#include "cpu.h"
#include "server.h"
#include "log.h"

void test()
{
	queues::memory::incoming::factory messages; // should be a task queue factory instead..??
	queues::memory::outgoing::factory responses;

	compute::cpu::cpu cpu(&messages);
	manager::manager manager(&responses);
	
	manager.add(&cpu);

	server::configuration configuration(&manager);
	server::server *server = new server::server(&configuration);

	if (server == NULL) return;
	if (server->initalised())
	{
		if (server->open())
		{
			if (server->start())
			{
				MSG message;
				while (GetMessage(&message, NULL, 0, 0) > 0)
				{
					TranslateMessage(&message);
					DispatchMessage(&message);
				};
			}
		}
	}

	delete server;
}

void test_dates()
{
	global::datetime a(2018, 10, 2);
	global::datetime b(2017, 10, 2);
	global::datetime c(2018, 9, 2);
	global::datetime d(2018, 10, 1);
	global::datetime e(2018, 10, 3);
	global::datetime f(2018, 11, 2);
	global::datetime g(2019, 9, 2);
	/*
	Log << (a < b) << " " << a.compare(b) << "\r\n";
	Log << (b < a) << " " << b.compare(a) << "\r\n";
	Log << (a < a) << " " << a.compare(a) << "\r\n";
	Log << (c < a) << " " << c.compare(a) << "\r\n";
	Log << (d < a) << " " << d.compare(a) << "\r\n";
	Log << (e < a) << " " << e.compare(a) << "\r\n";
	Log << (f < a) << " " << f.compare(a) << "\r\n";
	Log << (g < a) << " " << g.compare(a) << "\r\n";
	*/
}

int APIENTRY WinMain(HINSTANCE hInstance,
					 HINSTANCE hPrevInstance,
					 LPSTR     lpCmdLine,
					 int       nCmdShow)
{

	test_dates();
	// build in-memory queue
	// http client for test json post
	// database queue

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


	// need error console, that redirects to database
	// or console, or both

	// need json result/response system

	//curl --header "Content-Type: application/json"  --request POST --data-binary @body.json http://127.0.0.1:5555

	return 0;
}