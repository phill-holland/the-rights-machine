#include "parameter.h"
#include <vector>

#if !defined(__PARAMETERS)
#define __PARAMETERS

namespace web
{
	class parameters
	{
		static const unsigned long MAX = 15L;

	private:	
		std::vector<web::parameter> headers;

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

			headers.push_back(source);

			return true;
		}

		parameter get(unsigned long index)
		{
			return headers[index];
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