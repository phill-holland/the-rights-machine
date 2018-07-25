#include "string.h"
#include "json.h"

#if !defined(__ELEMENT)
#define __ELEMENT

namespace data
{
	namespace element
	{
		class element : public json
		{
		public:
			int elementID;
			int componentID;
			string value;

		public:
			element() { clear(); }
			element(json *parent) { clear(); json::parent(parent); }
			element(element const &source) { clear(); copy(source); }

			int identity() { return elementID; }

			void clear();

			string identifier() { return string("ELEMENT"); }
			bool add(custom::pair &source);

			void copy(element const &source);

		public:
			void output();

		public:
			element& operator=(const element& source)
			{
				this->copy((element&)source);
				return *this;
			}
		};
	};
};

#endif