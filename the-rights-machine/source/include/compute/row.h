#include "compute/interfaces/row.h"
#include "compute/header.h"

#if !defined(__ROW)
#define __ROW

namespace compute
{
	namespace cpu
	{
		class row : public ::compute::interfaces::row
		{
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

			int get(unsigned long idx);

			void set(header *source);
			bool set(unsigned long idx);

			int *raw() { return data; }

			bool copy(row const &source);

		public:
			unsigned long count() { return length; }
			bool first(header *destination) { *destination = top; return true; }

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
};

#endif