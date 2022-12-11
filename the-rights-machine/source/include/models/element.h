#include "core/string/string.h"
#include "core/custom/pair.h"
#include "parser/json/legacy/json.h"

#if !defined(_MODELS_ELEMENT)
#define _MODELS_ELEMENT

namespace models
{
	namespace element
	{
		class element : public json::request::json
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
			bool add(core::custom::pair source);

			void copy(element const &source);

		public:
			string output();

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