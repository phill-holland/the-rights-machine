#include "core/string/string.h"
#include "core/custom/pair.h"
#include "parser/json/legacy/json.h"

#if !defined(_MODELS_COMPONENT_LINE_COMPONENT)
#define _MODELS_COMPONENT_LINE_COMPONENT

namespace models
{
	namespace component
	{
		namespace line
		{
			class component : public json::request::json
			{
			public:
				int componentID;
				int lineID;
				string name;

			public:
				component() { clear(); }
				component(json *parent) { clear(); json::parent(parent); }
				component(component const &source) { clear(); copy(source); }

				int identity() { return componentID; }

				void clear();
				void copy(component const &source);

			public:
				string output();

			public:
				string identifier() { return string("COMPONENT"); }
				bool add(core::custom::pair source);

			public:
				component& operator=(const component& source)
				{
					this->copy((component&)source);
					return *this;
				}
			};
		};
	};
};

#endif