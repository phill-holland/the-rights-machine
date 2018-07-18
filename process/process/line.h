#include "fifo.h"
#include "components.h"
#include "string.h"
#include "datetime.h"
#include "zone.h"

#if !defined(__LINE)
#define __LINE

using namespace global;

namespace data
{
	namespace line
	{
		class line : public json, public zone::zone
		{
		public:
			enum TYPE { in = 0, out = 1 };

		public:
			int lineID;
			int itemID;
			//datetime start;
			//datetime end;
			int exclusivityID;
			int typeID;

		public:
			line() { clear(); }
			line(json *parent) { clear(); json::parent(parent); }
			line(line const &source) { clear(); copy(source); }

			void clear();

			//bool overlapped(line &source);

			//std::tuple<line, line, line> split(line &source);
			//std::vector<line> split(line &source);

			data::line::line spawn(datetime &start, datetime &end);

		public:
			void copy(line const &source);
			
		public:
			string identifier() { return string("LINE"); }
			bool add(custom::pair &source);

		//protected:
			//std::tuple<datetime, datetime, datetime, datetime> sort(datetime a, datetime b, datetime c, datetime d);

		public:
			line& operator=(const line& source)
			{
				this->copy((line&)source);
				return *this;
			}
		};
	};
};

#endif