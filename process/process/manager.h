#include <vector>
#include "queue.h"
#include "message.h"
#include "response.h"
#include "thread.h"
#include "task.h"
#include "compute.h"
#include "factory.h"

#if !defined(__MANAGER)
#define __MANAGER

namespace manager
{
	//class manager;

	/*
	class node
	{
		friend class manager;

	protected:
		queue::queue<data::message::message> *messages;
		queue::queue<data::response> *response; // not a queue, more a list/array

	public:
		node() { messages = NULL; response = NULL; }
		node(queue::queue<data::message::message> *mq, queue::queue<data::response> *rq)
		{
			messages = mq;
			response = rq;
		}
	};
	*/

	class manager : public ::queue::in<compute::task>, ::queue::out < ::queue::queue<data::response>>
	{
		//long total;
		long read;// , write;
		
		//long out;

		//custom::list<compute::compute*> *nodes;
		std::vector<compute::compute*> nodes;
		queue::factory<data::response> *factory;

		//task *nodes;
		bool init;

		mutex::token token;

	public:
		manager(queue::factory<data::response> *factory) { makeNull(); reset(factory); }
		~manager() { cleanup(); }

		bool initalised() { return init; }
		void reset(queue::factory <data::response> *factory);

		void add(compute::compute *source);
		/*
		{
			return engines->add(source);
		}
		*/
		// adds a compute task block
		//bool add(node &n);
		//bool push(compute::task &task);
		bool set(compute::task &source);

		bool get(::queue::queue<data::response> &destination)
		{
			destination = *factory->get();
		}

		//bool get(compute::task &destination) { return false; }
		/*
		{ 
			//return false; 

			mutex lock(token);

			if (read >= total) read = 0L;
			return engines->operator[read++]->push(task);

			//node n = nodes[read++];
			
			//if (n.messages != NULL)
			//{
				//if (!n.messages->set(*m)) return NULL;
			//}

			//return n.response;
		}
		*/
		//node *next();

		//queue::queue<data::response> *push(data::message::message *m);
	//protected:
		//bool get(compute::task &destination);

	protected:
		void makeNull();
		void cleanup();
	};
};

#endif