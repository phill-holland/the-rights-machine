
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

	public:
		header() { clear(); }
		header(header const &source) { clear(); copy(source); }
		~header() { }

		void clear()
		{
			messageID = 0;
			itemID = 0;
			lineID = 0;
		}

		void copy(header const &source)
		{
			messageID = source.messageID;
			itemID = source.itemID;
			lineID = source.lineID;
		}

	public:
		header& operator=(const header& source)
		{
			this->copy((header&)source);
			return *this;
		}
	};
};

#endif