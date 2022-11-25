#include "core/string/string.h"
#include "models/lines.h"
#include "core/custom/pair.h"
#include "parser/json/json.h"

#if !defined(_MODELS_ITEM)
#define _MODELS_ITEM

namespace models
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
			string output();

		public:
			string identifier() { return string("ITEM"); }
			bool add(core::custom::pair source);

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