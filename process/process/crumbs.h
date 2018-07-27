#include "string.h"

#if !defined(__CRUMBS)
#define __CRUMBS

namespace crumbs
{
	class crumbs
	{
		static const unsigned long DEPTH = 15UL;

		unsigned long depth;
		string *data;

		bool init;

	public:
		crumbs() { makeNull(); reset(); }
		~crumbs() { cleanup(); }

		bool initalised() { return init; }
		void reset();

		void clear();

		bool push(string value);
		bool pop();

		bool isempty() { return depth == 0UL; }

		string FQDN(string label = "");

	protected:
		void makeNull();
		void cleanup();
	};
};

#endif