#include "core/custom/datetime.h"
#include <vector>
#include <tuple>

#if !defined(_TYPES_ZONE)
#define _TYPES_ZONE

namespace types
{
	using namespace core::custom;
	
	class zone
	{
	public:
		datetime start;
		datetime end;

	public:
		zone() { start.clear(); end.clear(); }
		zone(zone const &source) { copy(source); }
		zone(datetime start, datetime end)
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
		std::tuple<datetime, datetime, datetime, datetime> sort(datetime a, datetime b, datetime c, datetime d);		
	};
};

#endif
