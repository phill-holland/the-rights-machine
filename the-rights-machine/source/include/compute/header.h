#include "core/string/string.h"
#include "types/zone.h"
#include "types/datetime.h"

#if !defined(_COMPUTE_HEADER)
#define _COMPUTE_HEADER

namespace compute
{
	using namespace types;

	class header : public zone
	{
	public:
		int messageID;
		int itemID;
		int lineID;
		int componentID;

	public:
		header(datetime start = datetime(), datetime end = datetime(),
			   int messageID = -1, int itemID = -1, 
			   int lineID = -1, int componentID = -1) : zone(start, end)
		{ 
			this->messageID = messageID;
			this->itemID = itemID;
			this->lineID = lineID;
			this->componentID = componentID;
		}

		header(header const &source) : zone(source) { clear(); copy(source); }
		~header() { }

		void clear();
		bool isempty();

		string serialize();
		
		void copy(header const &source);

	public:
		header& operator=(const header& source)
		{
			this->copy((header&)source);
			return *this;
		}
	};
};

#endif