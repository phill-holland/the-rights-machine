#include <vector>
#include "queues/interfaces/queue.h"
#include "message/message.h"
#include "models/response.h"
#include "core/threading/mutex.h"
#include "compute/task.h"
#include "compute/interfaces/compute.h"
#include "interfaces/factory.h"
#include "interfaces/chain.h"

#if !defined(__MANAGER)
#define __MANAGER

namespace manager
{
	class manager : public ::queue::in<compute::task>//, ::queue::out<custom::chain<data::response::response>>//::queue::queue<data::response::response>>
	{
		long read;

		std::vector<compute::compute*> nodes;
		queue::chain_factory<models::response::response> *factory;

		bool init;

		core::threading::mutex::token token;

	public:
		manager(queue::chain_factory<models::response::response> *factory) { makeNull(); reset(factory); }
		~manager() { cleanup(); }

		bool initalised() { return init; }
		void reset(queue::chain_factory <models::response::response> *factory);

		void add(compute::compute *source);

		bool set(compute::task &source);
		//bool get(custom::chain<data::response::response> &destination);//::queue::queue<data::response::response> &destination);

		// ***
		custom::chain<models::response::response> *get();
		// ***

	protected:
		void makeNull();
		void cleanup();
	};
};

#endif