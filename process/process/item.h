#include "string.h"
#include "lines.h"

#if !defined(__ITEM)
#define __ITEM

namespace data
{
	namespace item
	{
		class item : public json
		{
		public:
			int itemID;
			int messageID;
			string name;

		public:
			item() { clear(); }
			item(json *parent) { clear(); json::parent(parent); }
			item(item const &source) { clear(); copy(source); }

			void clear();
			void copy(item const &source);

		public:
			string identifier() { return string("ITEM"); }
			bool add(custom::pair &source);

		public:
			item& operator=(const item& source)
			{
				this->copy((item&)source);
				return *this;
			}
		};

		/*
		class item : public base
		{
		public:
			static const long MAX = 10L;

		private:
			bool init;

		public:
			data::lines::lines<MAX> *lines;

		public:
			item() { makeNull(); reset(); }
			item(item const &source) { makeNull(); reset(); copy(source); }
			~item() { cleanup(); }

			bool initalised() { return init; }
			void reset();

			void clear() override;

			void copy(item const &source);

		public:
			item& operator=(const item& source)
			{
				this->copy((item&)source);
				return *this;
			}

		protected:
			void makeNull();
			void cleanup();
		};
		*/
	};
};

#endif