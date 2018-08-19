#include "wsock.h"
#include <windows.h>
#include "configuration.h"
#include "manager.h"
#include "queues.h"
#include "cpu.h"
#include "server.h"
#include "errors.h"
#include "log.h"

#include "element.h"
#include "elements.h"
#include "guid.h"
#include "starter.h"

#include "grid.h"
#include "error.h"
#include "crumbs.h"
#include "parameters.h"
#include "queues.h"

void test()
{
	queues::memory::incoming::factory messages;
	queues::memory::outgoing::factory responses;

	compute::cpu::cpu cpu(&messages);
	if (!cpu.start()) return;

	manager::manager manager(&responses);	
	manager.add(&cpu);

	error::console::errors console;
	error::errors errors(&console);
	if (!errors.start()) return;

	server::configuration::configuration configuration(&manager, &errors);
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

int test_elements()
{
	data::elements::elements<10L> elements, copy;

	string names[] = {"english", "french", "german", "french", "urdu", "flemish", "polish" };
	for (int i = 0; i < 7; ++i)
	{
		data::element::element temp;
		
		temp.value = names[i];
		temp.componentID = i + 1;
		
		elements.temp = temp;
		elements.flush();
	}

	for (int i = 0; i < elements.count(); ++i)
	{
		data::element::element temp = elements[i];
		Log << temp.value << "\r\n";
	}

	copy = elements;
	Log << "copy count " << copy.count() << "\r\n";
	for (int i = 0; i < copy.count(); ++i)
	{
		data::element::element temp = elements[i];
		Log << "copy " << temp.value << "\r\n";
	}

	return 0;
}

void test_starter()
{
	server::starter s;
}

void test_grid()
{
	compute::grid *a, *b, *c;

	a = new compute::grid;
	b = new compute::grid;
	c = new compute::grid;

	delete c;
	delete b;
	delete a;
}

void test_error_type()
{
	::error::type::types *a;

	a = new ::error::type::types();

	delete a;
}


void test_crumbs()
{
	::crumbs::crumbs *c;

	c = new ::crumbs::crumbs();
	
	c->push(string("moo moo"));

	delete c;
}

void test_parameters()
{
	::web::parameters *a;

	a = new ::web::parameters();

	delete a;
}

void test_responses()
{
	/*
	data::response::response *response = new data::response::response();

	delete response;
	*/
	
	queues::memory::outgoing::factory *responses;

	responses = new queues::memory::outgoing::factory();

	custom::chain<data::response::response> *a = responses->get();

	delete responses;
	
}

int APIENTRY WinMain(HINSTANCE hInstance,
					 HINSTANCE hPrevInstance,
					 LPSTR     lpCmdLine,
					 int       nCmdShow)
{

	//guid::guid g;
	//Log << (string)g;
	//test_starter();
	//test_responses();
	//test_crumbs();
	//test_parameters();
	//test_error_type();
	//test_grid();
	test();
	//datetime tm(string("2018-06-02"));
	//test_elements();
	//test_dates();
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