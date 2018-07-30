#include <vector>
#include "queue.h"
#include "message.h"
#include "response.h"
#include "thread.h"
#include "task.h"
#include "compute.h"
#include "factory.h"

#include "chain.h"

#if !defined(__MANAGER)
#define __MANAGER

namespace manager
{
	class manager : public ::queue::in<compute::task>, ::queue::out<custom::chain<data::response::response>>//::queue::queue<data::response::response>>
	{
		long read;

		std::vector<compute::compute*> nodes;
		queue::chain_factory<data::response::response> *factory;

		bool init;

		mutex::token token;

	public:
		manager(queue::chain_factory<data::response::response> *factory) { makeNull(); reset(factory); }
		~manager() { cleanup(); }

		bool initalised() { return init; }
		void reset(queue::chain_factory <data::response::response> *factory);

		void add(compute::compute *source);

		bool set(compute::task &source);
		bool get(custom::chain<data::response::response> &destination);//::queue::queue<data::response::response> &destination);

	protected:
		void makeNull();
		void cleanup();
	};
};

#endif