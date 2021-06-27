#include "fifo.h"
#include "components.h"
#include "custom/string.h"
#include "datetime.h"
#include "zone.h"

#if !defined(__LINE)
#define __LINE

using namespace global;

namespace data
{
	namespace line
	{
		class line : public json::request::json, public zone::zone
		{
		public:
			enum TYPE { in = 0, out = 1 };

		public:
			int lineID;
			int itemID;
			int exclusivityID;
			int typeID;

		public:
			line() { clear(); }
			line(json *parent) { clear(); json::parent(parent); }
			line(line const &source) { clear(); copy(source); }

			int identity() { return lineID; }

			void clear();

			data::line::line spawn(datetime &start, datetime &end);

		public:
			void copy(line const &source);
			
		public:
			string output();

		public:
			string identifier() { return string("LINE"); }
			bool add(custom::pair source);

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