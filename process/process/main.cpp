#include "wsock.h"
#include <windows.h>
#include "configuration.h"
#include "manager.h"
#include "queues.h"
#include "cpu.h"
#include "server.h"

void test()
{
	queues::memory::incoming::factory messages;
	queues::memory::outgoing::factory responses;

	compute::cpu cpu(&messages);
	manager::manager manager;
	
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

int APIENTRY WinMain(HINSTANCE hInstance,
					 HINSTANCE hPrevInstance,
					 LPSTR     lpCmdLine,
					 int       nCmdShow)
{

	test();
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