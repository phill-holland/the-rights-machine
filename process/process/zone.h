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

		bool overlapped(zone &source)
		{
			return (!((start > source.end) || (source.start > end)));
		}

		std::vector<zone> split(zone &source)
		{
			auto swap = [](datetime &a, datetime &b)
			{
				datetime temp = a;
				a = b;
				b = temp;
			};

			std::vector<zone> result;

			if (end < start) swap(start, end);

			if ((start == source.start) && (end == source.end))
			{
				result.push_back(zone(start, end));
				return result;
			}

			std::tuple<global::datetime, global::datetime, global::datetime, global::datetime> dates = sort(start, end, source.start, source.end);

			global::datetime start = std::get<0>(dates), end = std::get<1>(dates);
			if (start != end) result.push_back(zone(start, end));

			global::datetime start = std::get<1>(dates), end = std::get<2>(dates);
			if (start != end) result.push_back(zone(start, end));

			global::datetime start = std::get<2>(dates), end = std::get<3>(dates);
			if (start != end) result.push_back(zone(start, end));

			return result;
		}

		void copy(zone const &source)
		{
			start = source.start;
			end = source.end;
		}

	public:
		zone &operator=(const zone& source)
		{
			this->copy((zone&)source);
			return *this;
		}

	protected:
		std::tuple<datetime, datetime, datetime, datetime> sort(datetime a, datetime b, datetime c, datetime d)
		{
			auto swap = [](datetime &a, datetime &b)
			{
				datetime temp = a;
				a = b;
				b = temp;
			};

			int swaps = 0;

			do
			{
				swaps = 0;
				if (b < a) { swap(a, b); ++swaps; }
				if (c < b) { swap(b, c); ++swaps; }
				if (d < c) { swap(c, d); ++swaps; }
			} while (swaps > 0);

			return std::tuple<datetime, datetime, datetime, datetime>(a, b, c, d);
		}
	};
};

#endif
