#include "elements.h"

# if !defined(__RESULT)
#define __RESULT

namespace compute
{
	class result
	{
		bool init;

	public:
		int messageID;
		int itemID;
		int lineID;
		int componentID;

		data::elements::elements<10L> elements;

	public:
		result() { makeNull(); reset(); }
		result(result const &source) { clear(); copy(source); }
		~result() { cleanup(); }

		bool initalised() { return init; }
		void reset();

		void clear();

		void copy(result const &source);

	protected:
		void makeNull();
		void cleanup();
	};
};

#endif