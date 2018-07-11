#include "queue.h"
#include "message.h"
#include "response.h"
#include "thread.h"

#if !defined(__MANAGER)
#define __MANAGER

namespace manager
{
	class manager;

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

	class manager
	{
		long total;
		long read, write;
		
		long out;

		node *nodes;
		bool init;

		mutex::token token;

	public:
		manager(long units) { makeNull(); reset(units); }
		~manager() { cleanup(); }

		bool initalised() { return init; }
		void reset(long units);

		bool add(node &n);

		node *next();

		queue::queue<data::response> *push(data::message::message *m);

	protected:
		void makeNull();
		void cleanup();
	};
};

#endif