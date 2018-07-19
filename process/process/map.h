#include "string.h"
#include "comparison.h"
#include <unordered_map>

#if !defined(__MAP)
#define __MAP

namespace mapping
{
	using namespace comparison;

	class mapper
	{
		std::unordered_map<string, int, hasher, equality> forward;
		std::unordered_map<int, string> reverse;

		int index;

	public:
		mapper() { reset(); }

		void reset()
		{
			index = 0;
		}

		void empty()
		{
			forward.clear();
			reverse.clear();
			index = 0;
		}

		bool push(string &name)
		{
			if (forward.find(name) != forward.end()) return false;

			forward[name] = index;
			reverse[index++] = name;
		}

		string map(int id)
		{
			return reverse.at(id);
		}

		int map(string &name)
		{
			return forward.at(name);
		}
	};
};

#endif
