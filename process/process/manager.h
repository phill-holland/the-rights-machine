#include <vector>
#include "queue.h"
#include "message.h"
#include "response.h"
#include "thread.h"
#include "task.h"
#include "compute.h"

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

	class manager
	{
		//long total;
		long read;// , write;
		
		//long out;

		//custom::list<compute::compute*> *nodes;
		std::vector<compute::compute*> nodes;

		//task *nodes;
		bool init;

		mutex::token token;

	public:
		manager() { makeNull(); reset(); }
		~manager() { cleanup(); }

		bool initalised() { return init; }
		void reset();

		void add(compute::compute *source);
		/*
		{
			return engines->add(source);
		}
		*/
		// adds a compute task block
		//bool add(node &n);
		bool push(compute::task &task);
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

	protected:
		void makeNull();
		void cleanup();
	};
};

#endif