#if !defined(__QUEUE)
#define __QUEUE

namespace queue
{
	class base
	{
	public:
		virtual bool flush() = 0;
	};

	template <class X> class in
	{
	public:
		virtual bool set(X &source) = 0;
	};

	template <class X> class out
	{
	public:
		virtual bool get(X &destination) = 0;
	};

	template <class X> class queue : public base, public in<X>, public out<X>
	{

	};
};

#endif