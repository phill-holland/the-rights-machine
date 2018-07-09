#include "fifo.h"
#include "components.h"
#include "string.h"
#include "datetime.h"

#if !defined(__LINE)
#define __LINE

using namespace global;

namespace data
{
	namespace line
	{
		class line : public json
		{
		public:
			int lineID;
			int itemID;
			datetime start;
			datetime end;
			int exclusivityID;
			int typeID;

		public:
			line() { clear(); }
			line(json *parent) { clear(); json::parent(parent); }
			line(line const &source) { clear(); copy(source); }

			void clear();
			void copy(line const &source);

		public:
			string identifier() { return string("LINE"); }
			bool add(custom::pair &source);

		public:
			line& operator=(const line& source)
			{
				this->copy((line&)source);
				return *this;
			}
		};

		/*
		class line : public base
		{
		public:
			static const long MAX = 5L;

		private:
			bool init;

		public:
			data::components::components<MAX> *components;

		public:
			line() { makeNull(); reset(); }
			line(line const &source) { makeNull(); reset(); copy(source); }
			~line() { cleanup(); }

			void reset();

			void clear() override;

			void copy(line const &source);

		public:
			line& operator=(const line& source)
			{
				this->copy((line&)source);
				return *this;
			}

		protected:
			void makeNull();
			void cleanup();
		};*/
	};
};

#endif