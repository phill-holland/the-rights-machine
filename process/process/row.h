#include "header.h"

#if !defined(__ROW)
#define __ROW

namespace compute
{
	class grid;

	class row
	{
		friend class grid;

	protected:
		static const unsigned long LENGTH = 255L;

	private:		
		unsigned long length;		

		header top;
		int *data;

		bool init;

	public:
		row(unsigned long length = LENGTH) { makeNull(); reset(length); }
		~row() { cleanup(); }

		bool initalised() { return init; }
		void reset(unsigned long length);

		void clear();

		void set(header &source);
		bool set(unsigned long idx);

		bool copy(row const &source);
		
	public:
		row& operator=(const row& source)
		{
			this->copy((row&)source);
			return *this;
		}

	protected:
		void makeNull();
		void cleanup();
	};
};

#endif