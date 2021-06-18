#include "custom/string.h"

#if !defined(__HEADER)
#define __HEADER

namespace compute
{
	class header
	{
	public:
		int messageID;
		int itemID;
		int lineID;
		int componentID;

	public:
		header(int messageID = 0, int itemID = 0, int lineID = 0, int componentID = 0) 
		{ 
			this->messageID = messageID;
			this->itemID = itemID;
			this->lineID = lineID;
			this->componentID = componentID;
		}

		header(header const &source) { clear(); copy(source); }
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