#include "pair.h"
#include "string.h"

#if !defined(__JSON)
#define __JSON

namespace data
{
	namespace json
	{
		namespace request
		{
			class json
			{
				json *_parent;

			public:
				json() { _parent = NULL; }

				virtual int identity() = 0;
				virtual void clear() = 0;
				virtual string identifier() = 0;
				virtual bool add(custom::pair &source) = 0;

			public:
				string FQDN();

				void parent(json *source) { _parent = source; }
				int progenitor() { if (_parent != NULL) return _parent->identity(); return 0; }
			};
		};

		namespace response
		{
			class json
			{
				json *_child;

			public:
				json() { _child = NULL; }

				string get_output()
				{
					return string("result");
				}

				void child(json *source) { _child = source; }

				virtual unsigned long count() = 0;
				virtual custom::pair& get() = 0;

			};
		};
	};
};

#endif