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
		std::unordered_map<int, int> parents;

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
			parents.clear();

			index = 0;
		}

		void push(string &name, int id, int parent)
		{
			if (forward.find(name) != forward.end())
			{
				forward[name] = index;
				reverse[index] = name;
				++index;
			}

			if (parent >= 0) parents[id] = parent;			
		}

		void push(int id, int parent)
		{
			parents[id] = parent;
		}

		string map(int idx)
		{
			return reverse.at(idx);
		}

		int map(string &name)
		{
			return forward.at(name);
		}

		int parent(int id)
		{
			return parents.at[id];
		}
		
		int maximum() { return forward.size; }
	};
};

#endif
