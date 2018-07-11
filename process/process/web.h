#include "buffer.h"
#include "string.h"
#include "list.h"

#if !defined(__WEB)
#define __WEB

namespace web
{
	class address
	{
	public:
		string server;
		string path;

		bool secure;

		long port;

	public:
		address() { reset(); }
		address(string &url) { reset(); get(url); }

		void reset();
		bool get(string &url);
	};


	class authentication
	{
	public:
		bool required;
		string username, password;

	public:
		authentication() { reset(); }

		void reset()
		{
			required = false;
			username.reset(); password.reset();
		}
	};

	class parameter
	{
	public:
		string name;
		string value;

	public:
		parameter() { }
		parameter(string _name, string _value) { name = _name; value = _value; }
	};

	class page
	{
		const static long length = 32768L;
		custom::list<parameter> *parameters;
		bool init;

	public:
		string *body;

	public:
		authentication authorization;

	public:
		long status;

	public:
		string url;
		string redirect;

	public:
		page() { makeNull(); reset(); }
		page(string &_url) { makeNull(); url.copy(_url); reset(); }
		~page() { cleanup(); }

		bool initalised() { return init; }

		void reset();

		bool set(string &name, string &value);
		string get(string &name);

	protected:
		void makeNull();
		void cleanup();
	};
};

#endif