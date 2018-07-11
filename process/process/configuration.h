#include "queue.h"
//#include "message.h"
//#include "response.h"
#include "manager.h"

#if !defined(__CONFIGURATION)
#define __CONFIGURATION

namespace server
{
	class configuration
	{
		manager::manager *manager;

	public:
		long port;
		long clients;

		//queue::queue<data::message::message> *output;
		//queue::queue<data::response> *input;

	protected:
		const static long PORT = 5555L;
		const static long CLIENTS = 1L;

	public:
		configuration(manager::manager *manager) { reset(manager); }
		configuration(configuration const &source) { copy(source); }

		void reset(manager::manager *manager);

		/*
		queue::queue<data::response> *push(data::message::message *m)
		{
			return manager->push(m);
		}
		*/
		manager::manager *get() { return manager; }

		void copy(configuration const &source);

	public:
		configuration& operator=(const configuration& source)
		{
			this->copy((configuration&)source);
			return *this;
		}
	};
};

#endif