#include "databases.h"
#include "string.h"

#if !defined(__RECORD)
#define __RECORD

namespace database
{
	namespace record
	{
		template <class X> class record
		{
		protected:
			char identifier[15L];

		public:
			virtual bool bind(database::recordset *recordset) = 0;
			virtual void set(X &source) = 0;

		public:
			void identify(string &source)
			{
				memset(identifier, 0, 15L);
				source.toChar(identifier, 15L);
			}

			string database::storage::message::generate()
			{
				string random = string::randBase64();
				random.toChar(identifier, 15L);

				return random;
			}
		};
	};
};

#endif