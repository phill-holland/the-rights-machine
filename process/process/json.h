#include "pair.h"
#include "string.h"

#if !defined(__JSON)
#define __JSON

namespace data
{
	class json
	{
		json *_parent;

	public:		
		json() { _parent = NULL; }

		void parent(json *source) { _parent = source; }
		//json *parent() { return _parent; }

		string FQDN();

		virtual int identity() = 0;
		int progenitor() { if (_parent != NULL) return _parent->identity(); return 0; }

		//virtual void identity(int id) = 0;

		//bool walk(string *parents, long depth);
		// some sort of FQDN compare here
		virtual void clear() = 0;
		virtual string identifier() = 0;
		virtual bool add(custom::pair &source) = 0;
	};
};

#endif