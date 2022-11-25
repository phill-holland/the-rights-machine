#include "database/databases.h"
#include "core/string/string.h"

#if !defined(__RECORD)
#define __RECORD

namespace database
{
	namespace record
	{
		template <class X> class record
		{
		public:
			virtual bool bind(database::recordset *recordset) = 0;
			virtual void set(X &source) = 0;
		};
	};
};

#endif