#include "custom/string.h"
#include "lines.h"

#if !defined(__ITEM)
#define __ITEM

namespace data
{
	namespace item
	{
		class item : public json::request::json
		{
		public:
			int itemID;
			int messageID;
			string name;

		public:
			item() { clear(); }
			item(json *parent) { clear(); json::parent(parent); }
			item(item const &source) { clear(); copy(source); }

			int identity() { return itemID; }

			void clear();
			void copy(item const &source);

		public:
			void output();

		public:
			string identifier() { return string("ITEM"); }
			bool add(custom::pair source);

		public:
			item& operator=(const item& source)
			{
				this->copy((item&)source);
				return *this;
			}
		};
	};
};

#endif