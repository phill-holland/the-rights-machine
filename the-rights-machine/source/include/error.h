#include "json.h"
#include "string.h"
#include "comparison.h"
#include <unordered_map>

#if !defined(__ERROR)
#define __ERROR

namespace error
{
	class error
	{
	public:
		string name;

	public:
		error(string name = "") { reset(name); }

		void reset(string name = "")
		{
			this->name = name;
		}
	};

	namespace type
	{
		class types;

		class type : public data::json::response::json
		{
			friend class types;

		protected:
			long code;

		public:			
			string name;
			string description;

		public:
			type(string name = "", string description = "")
			{
				code = 0L;
				this->name = name;
				this->description = description;
			}

			string get()
			{
				return name + " : " + description;
			}

		public:
			string identifier() { return string("error"); }

			unsigned long pairs();
			custom::pair pull(unsigned long index);

		public:
			operator string() { return get(); }
		};

		using namespace comparison;

		class types
		{
			static const unsigned long LENGTH = 255UL;

			unsigned long length;
			type **errors;

			std::unordered_map<string, long, hasher, equality> reverse;

			bool init;

		public:
			types(queue::out<::error::type::type> *source = NULL) { makeNull(); reset(source); }
			~types() { cleanup(); }
			
			bool initalised() { return init; }
			void reset(queue::out<::error::type::type> *source = NULL);

			bool push(type &t);
						
			type lookup(::error::error &error);

		protected:
			void makeNull();
			void cleanup();
		};
	};	
};

#endif