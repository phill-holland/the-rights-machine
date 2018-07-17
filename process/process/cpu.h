#include "compute.h"
#include "queue.h"
#include "message.h"
#include "response.h"
#include "fifo.h"
#include "task.h"
#include "factory.h"
#include "thread.h"

#include "line.h"

#if !defined(__CPU)
#define __CPU

namespace compute
{
	namespace cpu
	{
		class block
		{
		protected:
			class header
			{
			public:
				int messageID;
				int itemID;
				int lineID;
			};

			const static unsigned long WIDTH = 255;
			const static unsigned long HEIGHT = 255;

		private:
			unsigned long width, height;

			bool init;


			// add a function to lines in message?
			//void populate(int lineID, block *output);
		//public:
			//header *headers;// [255];
			int *data;// [255][255];

			//data::line::line in[255], out[255];

		public:
			block(unsigned long width = WIDTH, unsigned long height = HEIGHT) { makeNull(); reset(width, height); }
			~block() { cleanup(); }

			bool initalised() { return init; }
			void reset(unsigned long width, unsigned long height);

			void clear();

			void set(unsigned long x, unsigned long y)
			{
				if ((x >= width) || (y >= height)) return;
				data[(y * height) + x] = 1;
			}

			void minus(block &right);
			//bool add(const data::line::line &source, const data::c);
		//protected:
			//void split(void *lines)
			//{
				// loop through lines, add to in or out array


			//}
		public:
			block& operator-(const block &source)
			{
				//this->copy((datetime&)source);
				return *this;
			}

			/*
			block &operator+(const data::line::line &source)
			{
				add(source);
				return *this;
			}
			*/
		protected:
			void makeNull();
			void cleanup();
		};

		/*
		class inlines
		{
		public:
			int lineID; // have two lines after a split with same ID
			datetime start, end;

		};
		*/
		class cpu : public compute, public thread
		{
			::queue::queue<::compute::task> *queue;

			bool init;

		protected:
			void sort(datetime &a, datetime &b, datetime &c, datetime &d);
			void process();

		public:
			DWORD WINAPI background(thread *bt);

		public:
			cpu(::queue::factory<::compute::task> *factory) { makeNull(); reset(factory); }
			~cpu() { cleanup(); }

			bool initalised() { return init; }
			void reset(::queue::factory<::compute::task> *factory);

			bool set(::compute::task &source) 
			{ 
				return queue->set(source); 
				process();
			}
			bool flush() { return queue->flush(); }

		protected:
			bool get(::compute::task &destination) { return queue->get(destination); }

		protected:
			void makeNull();
			void cleanup();
		};
	};
};

#endif