#include "buffer.h"
#include "string.h"
#include "list.h"
#include "parameter.h"
#include "parameters.h"
#include "authentication.h"
#include "address.h"

#if !defined(__PAGE)
#define __PAGE

namespace web
{
	class page
	{
		const static long length = 32768L;
		web::parameters parameters;
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