#include "queue.h"
#include "message.h"
#include "response.h"
#include "chain.h"
#include "notification.h"
#include "inquiry.h"
#include "json.h"
#include "comparison.h"
#include <unordered_map>

#if !defined(__TASK)
#define __TASK

namespace compute
{
	using namespace data;
	using namespace comparison;

	class task : public json::request::json
	{
		std::unordered_map<string, data::json::request::json *, hasher, equality> hash;
		std::unordered_map<string, queue::base *, hasher, equality> queue_hash;

		bool init;

	public:
		data::message::inquiry inquiry;
		data::message::message message;
		custom::chain<data::response::response> *response;
		notification::notification *notify;

	public:
		task() { makeNull(); reset(); }
		~task() { cleanup(); }

		bool initalised() { return init; }
		void reset();
				
	public:
		int identity() { return 0; }
		void clear();

		string identifier() { return string("TASK"); }
		bool add(custom::pair source);

		queue::base *findQ(string FQDN);
		data::json::request::json *find(string FQDN);

	protected:
		void makeNull()
		{
			response = NULL;
			notify = NULL;
		}

		void cleanup() { }
	};
};

#endif