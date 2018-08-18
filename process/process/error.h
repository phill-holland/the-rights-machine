#include "string.h"
#include "comparison.h"
#include <unordered_map>

#if !defined(__ERROR)
#define __ERROR

namespace error
{
	/*
	namespace type
	{
		class type;
	};
	*/
	class error
	{
		//enum CODE { NONE = 0, ALLOCATION = 1, OTHER = 2, UNKNOWN = 3 };

	public:
		string name;
		//CODE code;
		//string description;

	public:
		error(string name = "") { reset(name); }

		void reset(string name = "")
		{
			this->name = name;
			//code = CODE::NONE;
			//description = "";
		}

		//string to()
		//{
			//const string map[] = { string("NONE"), string("ALLOCATION"), string("OTHER"), string("UNKNOWN") };

			//string result = map[(int)code];
			//result += ":";
			//result += description;
			
			//return result;

			//return name;
		//}

		//string to(::error::type::type &t)
		//{
		//}

	//public:
	//	operator string() { return to(); }
	};

	namespace type
	{
		class types;

		class type
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
			types() { makeNull(); reset(); }
			~types() { cleanup(); }

			bool initalised() { return init; }
			void reset();

			bool push(type &t);
			
			type lookup(::error::error &error);

		protected:
			void makeNull();
			void cleanup();
		};
	};	
};

#endif