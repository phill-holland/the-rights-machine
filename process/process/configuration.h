#include "queue.h"
#include "message.h"
#include "response.h"

#if !defined(__CONFIGURATION)
#define __CONFIGURATION

namespace server
{
	class configuration
	{
	public:
		long port;
		long clients;

		queue::queue<data::message::message> *output;
		queue::queue<data::response> *input;

	protected:
		const static long PORT = 5555L;
		const static long CLIENTS = 1L;

	public:
		configuration() { reset(); }
		configuration(configuration const &source) { copy(source); }

		void reset();

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