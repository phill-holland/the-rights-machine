#include "core/string/string.h"
#include "types/zone.h"
#include "core/custom/datetime.h"

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
		header(datetime start, datetime end,
			   int messageID, int itemID, 
			   int lineID, int componentID) : zone(start, end)
		{ 
			this->messageID = messageID;
			this->itemID = itemID;
			this->lineID = lineID;
			this->componentID = componentID;
		}
		header() : zone(datetime(), datetime())
		{ 
			this->messageID = -1;
			this->itemID = -1;
			this->lineID = -1;
			this->componentID = -1;
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