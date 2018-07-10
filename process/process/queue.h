#if !defined(__QUEUE)
#define __QUEUE

namespace queue
{
	class base
	{
	public:
		virtual bool flush() = 0;
	};

	template <class X> class queue : public base
	{
	public:
		virtual bool get(X &destination) = 0;
		virtual bool set(X &source) = 0;

		//virtual bool put(X *source) = 0;
		//virtual bool get(X *destination) = 0;

		//virtual bool flush() = 0;
	};
};

// json, pumps data into a "queue" - either in-memory, or database
// process, then accesses this queue

// need two queues, input/output - for all done
#endif