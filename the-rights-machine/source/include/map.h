#include "custom/string.h"
#include "comparison.h"
#include <unordered_map>
#include "log.h"

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
		mapper(mapper const &source) { reset(); copy(source); }

		void reset()
		{
			index = 0;
		}

		void empty()
		{
			forward.clear();
			reverse.clear();
			parents.clear();
			identities.clear();

			index = 0;
		}

		void push(string &name, int id, int parent)
		{
			if(identities.find(id) == identities.end())
			{
				identities[id] = name;
			}

			if (forward.find(name) == forward.end())
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
			try
			{
				return reverse.at(idx);
			}
			catch (std::out_of_range)
			{
				return string("");
			}
		}

		string name(int id)
		{
			try
			{
				return identities.at(id);
			}
			catch (std::out_of_range)
			{
				return string("");
			}
		}

		int map(string &name)
		{
			try
			{
				return forward.at(name);
			}
			catch (std::out_of_range)
			{
				return -1;
			}
		}

		int parent(int id)
		{
			try
			{
				return parents.at(id);
			}
			catch (std::out_of_range)
			{
				return -1;
			}
		}		

		int maximum() { return (int)forward.size(); }

		void copy(mapper const &source)
		{
			empty();

			forward = source.forward;
			reverse = source.reverse;
			identities = source.identities;
			parents = source.parents;

			index = source.index;
		}

	public:
		mapper operator=(const mapper& source)
		{
			this->copy((mapper&)source);
			return *this;
		}
	};
};

#endif
