#include "custom/string.h"
#include "zone.h"
#include "datetime.h"

#if !defined(__HEADER)
#define __HEADER

namespace compute
{
	class header : public zone::zone
	{
	public:
		int messageID;
		int itemID;
		int lineID;
		int componentID;

	public:
		header(global::datetime start = global::datetime(), global::datetime end = global::datetime(),
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