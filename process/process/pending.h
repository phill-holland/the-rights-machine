#include "string.h"
#include "datetime.h"
#include <unordered_map>
#include "comparison.h"

#if !defined(__PENDING)
#define __PENDING

namespace pending
{
	using namespace global;
	using namespace comparison;

	class pending;

	class waiting
	{
		friend class pending;

	public:
		string guid;
		string user;

	protected:
		datetime created;

	public:
		waiting(string guid = "", string user = "")
		{
			this->guid = guid;
			this->user = user;

			created = datetime::now();
		}

		bool validate()
		{
			if (guid.count() == 0L) return false;
			if (user.count() == 0L) return false;

			return true;
		}
	};

	// include thread to check for age of pending records
	// delete is over half an hour

	// do same in server class, for unrequest results

	class pending
	{
		std::unordered_map<string, waiting, hasher, equality> map;

		bool init;

	public:
		pending() { makeNull(); reset(); }
		~pending() { cleanup(); }

		bool initalised() { return init; }
		void reset();

		bool add(::pending::waiting wait);
		
		bool remove(::pending::waiting &wait);

		bool contains(::pending::waiting &wait);

	protected:
		void makeNull();
		void cleanup();
	};
};

#endif