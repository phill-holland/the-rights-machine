#include "string.h"
#include "databases.h"
#include "guid.h"

#if !defined(__FILE)
#define __FILE

namespace file
{
	template <class T> class file
	{
	public:
		virtual bool open(database::settings &settings) = 0;
		virtual bool close() = 0;

		virtual bool read(T &destination) = 0;
		virtual bool write(T &source) = 0;

	public:
	/*
		GUID generate()
		{
			return (GUID)guid::guid();
		}*/
	};
};

#endif