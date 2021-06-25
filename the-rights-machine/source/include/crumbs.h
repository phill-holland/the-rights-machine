#include "custom/string.h"
#include <vector>

#if !defined(__CRUMBS)
#define __CRUMBS

namespace crumbs
{
	class crumbs
	{
		static const unsigned long DEPTH = 15UL;

		unsigned long depth;
		//std::vector<string> data;
		string **data;

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

		unsigned long size() { return depth; }

	protected:
		void makeNull();
		void cleanup();
	};
};

#endif