#include "string.h"
#include "elements.h"
#include "json.h"

#if !defined(__COMPONENT)
#define __COMPONENT

namespace data
{
	namespace component
	{
		class component : public json
		{
		public:
			int componentID;
			int lineID;
			string value;

		public:
			component() { clear(); }
			component(json *parent) { clear(); json::parent(parent); }
			component(component const &source) { clear(); copy(source); }

			void clear();
			void copy(component const &source);

		public:
			string identifier() { return string("COMPONENT"); }
			bool add(custom::pair &source);

		public:
			component& operator=(const component& source)
			{
				this->copy((component&)source);
				return *this;
			}
		};

		/*
		class component : public base
		{
		public:
			static const long MAX = 5L;

		private:
			bool init;

		public:
			data::elements::elements<MAX> *elements;

		public:
			component() { makeNull();  reset(); }
			component(component const &source) { clear(); copy(source); }
			~component() { cleanup(); }

			void reset();

			void clear() override;

			void copy(component const &source);

		public:
			component& operator=(const component& source)
			{
				this->copy((component&)source);
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