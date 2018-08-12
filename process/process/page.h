#include "string.h"
#include "list.h"
#include "parameter.h"
#include "parameters.h"
#include "authentication.h"
#include "address.h"

#if !defined(__PAGE)
#define __PAGE

namespace http
{
	namespace client
	{
		class client;
	};
};

namespace web
{
	class page
	{
		friend class http::client::client;

		string *body;

		const static long length = 32768L;
		web::parameters parameters;
		bool init;

	public:
		authentication authorization;

	public:
		long status;

	public:
		string url;
		string redirect;

	public:
		page() { makeNull(); reset(); }
		page(string &addr) { makeNull(); url.copy(addr); reset(); }
		~page() { cleanup(); }

		bool initalised() { return init; }

		void reset();

		bool set(string &name, string &value);
		string get(string &name);

		void data(string &source) { *body = source; }
		string data() { return *body; }

		long size() { return (long)body->size(); }

	protected:
		void makeNull();
		void cleanup();
	};
};

#endif