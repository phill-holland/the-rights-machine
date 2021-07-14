#include "datetime.h"
#include <vector>
#include <tuple>

#if !defined(__ZONE)
#define __ZONE

namespace zone
{
	class zone
	{
	public:
		global::datetime start;
		global::datetime end;

	public:
		zone() { start.clear(); end.clear(); }
		zone(zone const &source) { copy(source); }
		zone(global::datetime start, global::datetime end)
		{
			this->start = start;
			this->end = end;
		}

		bool overlapped(zone &source);
		std::vector<zone> split(zone &source);
		
		void copy(zone const &source);
		
	public:
		zone &operator=(const zone& source)
		{
			this->copy((zone&)source);
			return *this;
		}

	protected:
		std::tuple<global::datetime, global::datetime, global::datetime, global::datetime> sort(global::datetime a, global::datetime b, global::datetime c, global::datetime d);		
	};
};

#endif
