#include "gtest/gtest.h"

/*

void test_dates()
{
	global::datetime a(2018, 10, 2);
	global::datetime b(2017, 10, 2);
	global::datetime c(2018, 9, 2);
	global::datetime d(2018, 10, 1);
	global::datetime e(2018, 10, 3);
	global::datetime f(2018, 11, 2);
	global::datetime g(2019, 9, 2);
	
	Log << (a < b) << " " << a.compare(b) << "\r\n";
	Log << (b < a) << " " << b.compare(a) << "\r\n";
	Log << (a < a) << " " << a.compare(a) << "\r\n";
	Log << (c < a) << " " << c.compare(a) << "\r\n";
	Log << (d < a) << " " << d.compare(a) << "\r\n";
	Log << (e < a) << " " << e.compare(a) << "\r\n";
	Log << (f < a) << " " << f.compare(a) << "\r\n";
	Log << (g < a) << " " << g.compare(a) << "\r\n";
	
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
	//string location = "DRIVER=SQL Server Native Client 11.0;SERVER=DESKTOP-DHP798L;UID=sa;PWD=Funjuice97?;WSID=MSSQLSERVER;DATABASE=Process;";

	messaging::memory::memory messages;

	server::starter s(&messages);
}

void test_grid()
{
	compute::cpu::grid *a, *b, *c;

	a = new compute::cpu::grid;
	b = new compute::cpu::grid;
	c = new compute::cpu::grid;

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
	//data::response::response *response = new data::response::response();

	//delete response;
	
	
	queues::memory::outgoing::factory *responses;

	responses = new queues::memory::outgoing::factory();

	custom::chain<data::response::response> *a = responses->get();

	delete responses;
	
}

void test_mappings()
{
	mapping::mapper map;

	map.push(string("woof"), 12, 0);

	string result = map.map(0);
	string moo = map.map(10);

	mapping::mapper mine = map;

	string result1 = mine.map(0);
	string moo1 = mine.map(10);

}

void test_rows()
{
	unsigned long width = 255;
	unsigned long height = 255;

	compute::cpu::row **rows = new compute::cpu::row*[height];
	if (rows == NULL) return;
	for (unsigned long i = 0UL; i < height; ++i) rows[i] = NULL;

	for (unsigned long i = 0UL; i < height; ++i)
	{
		rows[i] = new compute::cpu::row(width);
		if (rows[i] == NULL) return;
		if (!rows[i]->initalised()) return;
	}

	for (unsigned long y = 0UL; y < height; ++y)
	{
		for (unsigned long x = 0UL; x < width; ++x)
		{
			(*rows[y]).set(x);
		}
	}
// ***
	Log << "done\r\n";
	for (unsigned long i = 0UL; i < height; ++i)
	{
		delete rows[i];
	}
}

void test_cpu()
{
	compute::cpu::processor *cpu = new compute::cpu::processor(255, 255);
	if (cpu == NULL) return;

	delete cpu;
}

void test_dt()
{
	global::datetime now = global::datetime::now();

	global::datetime bob(0, 7, 0);

	datetime result = now + bob;

	Log << (string)result;
}

void test_users_db()
{
	database::odbc::factory::connection connections;
	database::odbc::factory::recordset recordsets;

	string location = "DRIVER=SQL Server Native Client 11.0;SERVER=DESKTOP-DHP798L;UID=sa;PWD=Funjuice97?;WSID=MSSQLSERVER;DATABASE=Process;";

	database::settings settings(location, &connections, &recordsets);

	data::users users(settings);
	
	data::user result = users.get(1);
	if (!result.isempty())
	{
		result.output();
	}
}
*/
/*
#include <gtest/gtest.h>

// Demonstrate some basic assertions.
TEST(HelloTest, BasicAssertions) {
  // Expect two strings not to be equal.
  EXPECT_STRNE("hello", "world");
  // Expect equality.
  EXPECT_EQ(7 * 6, 42);
}
*/
int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}