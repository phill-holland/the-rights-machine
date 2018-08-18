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
		//std::vector<web::parameter> headers;
		web::parameter **headers;
		std::unordered_map<string, int, hasher, equality> map;

		unsigned long length;

		bool init;

	public:
		parameters() { makeNull(); reset(); }
		~parameters() { cleanup(); }

		bool initalised() { return init; }
		void reset();

		void clear();

		bool add(parameter &source);

		bool exists(string &name);

		parameter& get(unsigned long index);
		string get(string &name);

		long count();

	public:
		parameter& operator[](int index)
		{
			return get((unsigned long)index);
		}

	protected:
		void makeNull();
		void cleanup();
	};
};

#endif