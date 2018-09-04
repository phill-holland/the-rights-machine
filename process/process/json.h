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
				virtual json *find(string FQDN) { return NULL; }

			public:
				bool parse(string json);

				string FQDN();

				void parent(json *source) { _parent = source; }
				int progenitor() { if (_parent != NULL) return _parent->identity(); return 0; }
			};
		};

		namespace response
		{
			class json
			{
				json *_child; // this may have many children, needs to be modified

			public:
				json() { _child = NULL; }

				string extract();

				void child(json *source) { _child = source; }

				virtual string identifier() = 0;
				virtual unsigned long pairs() = 0;
				virtual custom::pair pull(unsigned long index) = 0;
			};
		};
	};
};

#endif