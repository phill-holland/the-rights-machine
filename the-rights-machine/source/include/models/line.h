//#include "core/queue/fifo.h"
#include "core/string/string.h"
#include "types/datetime.h"
#include "types/zone.h"

#if !defined(_MODLS_LINE_LINE)
#define __MODELS_LINE_LINE

using namespace types;

namespace models
{
	namespace line
	{
		class line : /*public json::request::json,*/ public zone
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
			//line(json *parent) { clear(); json::parent(parent); }
			line(line const &source) { clear(); copy(source); }

			int identity() { return lineID; }

			void clear();

			line spawn(datetime &start, datetime &end);

		public:
			void copy(line const &source);
			
		public:
			string output();

		//public:
		//	string identifier() { return string("LINE"); }
		//	bool add(custom::pair source);

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