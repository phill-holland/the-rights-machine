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
		std::unordered_map<int, string> identities;
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
			identities.clear();
			parents.clear();
			index = 0;
		}

		void push(string &name, int id, int parent = -1)
		{
			if (forward.find(name) != forward.end())
			{
				if (parent >= 0)
				{
					if (parents.find(parent) == parents.end()) parents[parent] = 0;
					forward[name] = parents[parent];
					reverse[parents[parent]] = name;
					parents[parent] = parents[parent] + 1;
				}
				else
				{
					forward[name] = index;
					reverse[index] = name;
					++index;
				}
			}

			identities[id] = name;
		}

		string map(int idx)
		{
			return reverse.at(idx);
		}

		int map(string &name)
		{
			return forward.at(name);
		}

		string identity(int id)
		{
			return identities.at(id);
		}

		int maximum() { return forward.size; }
	};
};

#endif
