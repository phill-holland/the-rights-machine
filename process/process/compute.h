#include "message.h"
#include "task.h"

#if !defined(__COMPUTE)
#define __COMPUTE

namespace compute
{
	/*
	class pair
	{
	public:
		queue::queue<data::message::message> *messages; // output
		queue::queue <data::response> *responses; // input, single queue, for when process is finished, need to link messageID to response queue, inside compute
												  // queue now held in server, only it's message responses are sent to this server
	};
	*/

	/*
	class wrapper
	{
	public:
		data::message::message message;
		queue::queue<data::response> *response;

		// need copyt functions here, respoonse remains a ptr copy
		// message needs deep copy
	};
	*/
	/*
	class compute2 : public ::queue::queue<::data::message::message>
	{
	};

	class compute3 : public ::queue::queue<::compute::task>
	{
	};
	*/

	// why cannot the message class include the response queue as a pointer??
	class compute
	{
	public:
		//virtual bool calculate(data::message::message *source, int length) = 0;
		virtual bool push(::compute::task &task) = 0;

		//virtual bool push(pair *source) = 0;
	//	virtual queue::queue<data::message::message> *//queue::queue<data::response> 
	};
};

#endif