#include "parameter.h"
#include "comparison.h"
#include <vector>
#include <unordered_map>

#if !defined(__PARAMETERS)
#define __PARAMETERS

namespace web
{
	using namespace comparison;

	class parameters
	{
		static const unsigned long MAX = 15L;

	private:	
		std::vector<web::parameter> headers;
		std::unordered_map<string, int, hasher, equality> map;

		bool init;

	public:
		parameters() { makeNull(); reset(); }
		~parameters() { cleanup(); }

		bool initalised() { return init; }
		void reset();

		void clear() { return headers.clear(); }

		bool add(parameter &source)
		{
			if (count() >= MAX) return false;
			
			if (map.find(source.name) == map.end()) map[source.name] = (int)headers.size();
			headers.push_back(source);

			return true;
		}

		bool exists(string &name)
		{
			return (map.find(name) != map.end());
		}

		parameter& get(unsigned long index)
		{
			return headers[index];
		}

		string get(string &name)
		{
			if (map.find(name) != map.end()) return headers[map[name]].value;

			return string("");
		}

		long count() { return (unsigned long)headers.size(); }

	public:
		parameter& operator[](int index)
		{
			return get((unsigned long)index);
		}

	protected:
		void makeNull() { }
		void cleanup() { }
	};
};

#endif