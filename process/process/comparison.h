#include "string.h"

#if !defined(__COMPARISON)
#define __COMPARISON

namespace comparison
{
	struct equality
	{
		bool operator()(const string &str1, const string &str2) const
		{
			return ((string&)str1).icompare((string&)str2);
		}
	};

	struct hasher
	{
		std::size_t operator()(const string& k) const
		{
			return (std::size_t)(((string&)k).upper()).hash();
		}
	};
};

#endif