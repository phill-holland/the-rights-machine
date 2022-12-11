#if !defined(_COMPUTE_INTERFACES_ROW)
#define _COMPUTE_INTERFACES_ROW

namespace compute
{
	class header;

	namespace interfaces
	{
		class row
		{
		public:
			virtual void clear() = 0;

			virtual int get(unsigned long idx) = 0;

			virtual void set(header *source) = 0;
			virtual bool set(unsigned long idx) = 0;

			virtual int *raw() = 0;

			virtual bool first(header *destinatiom) = 0;

			virtual unsigned long count() = 0;
		};
	};
};

#endif