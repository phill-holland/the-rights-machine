#include "core/string/string.h"
//#include "elements.h"
//#include "json.h"

#if !defined(_MODELS_COMPONENT_QUERY_COMPONENT)
#define _MODELS_COMPONENT_QUERY_COMPONENT

namespace models
{
	namespace component
	{
		namespace query
		{
			class component// : public json::request::json
			{
			public:
				int componentID;
				int queryID;
				string name;

			public:
				component() { clear(); }
			//	component(json *parent) { clear(); json::parent(parent); }
				component(component const &source) { clear(); copy(source); }

				int identity() { return componentID; }

				void clear();
				void copy(component const &source);

			public:
				string output();

			//public:
			//	string identifier() { return string("COMPONENT"); }
			//	bool add(custom::pair source);

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