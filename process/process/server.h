#include "wsock.h"
#include <windows.h>
#include "thread.h"
#include "message.h"
//#include "items.h"
//#include "item.h"
//#include "lines.h"
//#include "line.h"
//#include "components.h"
//#include "component.h"
#include "configuration.h"
//#include "allocator.h"

// need error class, to create response error json/or message

// upon connection, create GUID for message return

// http  functions, two one for post, one to get results
// JSON post /input
// JSON GET // wait for result in response queue, per items

// need item limit depending on userID account

// need timeout, for inactivity on connection

// COUNT LINES IN JSON, RETURN LINE NUMBER IF ERROR

// check http headers
// check content-length

// PRE DECLARE GIANT ARRAY OF COMPONENTS
// AND HAVE FIFOS WITH POINTERS ONLY
// COMPONENT MEMORY MANAGER ..??

// NEED LOCKS FOR THAT -- WELL JUST FOR THE LAST PTR

// HAVE SMALLER QUEUE BUFFER FOR SERVER,
// ONE MESSAGE CAN BE SPLIT OVER MANY QUEUE INSERTS, PER ITEM

#if !defined(__SERVER)
#define __SERVER

namespace server
{
	/*
	class queue_item
	{
		// this is a new message!!!
		// move find function into message class

		int component_counter;
		int element_counter;
		int line_counter;
		int item_counter;
		int message_counter;
		//int item_counter;

	public:
		data::message::base message;
		data::item::base item;

		allocator::allocator<data::line::base, 10L> lines;
		allocator::allocator<data::component::base, 10L> components;
		allocator::allocator<data::element::element, 10L> elements;

	public:
		void add(data::message::base &source)
		{
			source.messageID = message_counter;
			message.copy(source);
			++message_counter;
			line_counter = 1;
			item_counter = 1;
			element_counter = 1;
		}

		void add(data::item::item &source)
		{
			source.messageID = message_counter;
			source.itemID = item_counter++;
			item.copy(source);
		}

		void add(data::line::line &source)
		{
			source.lineID = line_counter++;
			source.itemID = item_counter;
			lines.set(source);
		}

		void add(data::component::base &source)
		{
			source.componentID = component_counter++;
			source.lineID = line_counter;
			components.set(source);
		}

		void add(data::element::element &source)
		{
			source.elementID = element_counter++;
			source.componentID = component_counter;
			elements.set(source);
		}
	};
	*/
	// ****
	class client;

	class listener : public thread
	{
		static const long RECEIVING = 16384L;
		static const long ERRORS = 5L;
		static const long DEPTH = 15L;
		static const long LENGTH = 255L;

		char receiving[RECEIVING];	

		long errors;

		char parents[DEPTH][LENGTH];

		long depth;

		bool quotes;
		bool left;

		long brackets, squares;

	
		bool header;
		int h_index;

		char label[LENGTH];
		long idx_label;

		char value[LENGTH];
		long idx_value;

		data::message::message message;

		/*
		data::message::base message;
		data::items::base items;
		data::item::base item;
		data::lines::base lines;
		data::line::base line;
		data::components::base components;
		data::component::base component;
		data::elements::base elements;
		data::element::element element;
		*/
		data::json *current;

		/*
		data::items::items<10L> temp_items;
		data::item::item temp_item;
		data::lines::lines<10L> temp_lines;
		data::line::line temp_line;
		data::components::components<10L> temp_comps;
		*/
		client *c;


	public:
		DWORD WINAPI background(thread *bt);

	public:
		listener(client *source) { reset(source); }

		void reset(client *source);

		void clear();

	protected:
		//data::json *find(string label);
		string FQDN(string label);
		void extract(string parent, string label, string value);		
	};
		
	class client : public ::wsock::client
	{
	public:
		enum ERRORS { None = 0,
					  Read = 1,
					  Write = 2,
					  Start = 3,
					  Receiving = 4,
					  End = 5,
					  Unexpected = 6,
					  Peek = 7,
					  Ping = 8,
					  Mismatch = 9,
					  Changed = 10,
					  Fail = 11
					};

	protected:
		class states;

		class pending
		{
			friend class states;

		protected:
			enum STATE { Idle = 0, Pending = 1, Finished = 2, Ready = 3 };

			long identity;
			STATE state;

		public:
			pending() { reset(); }
			void reset() { identity = 0L; state = STATE::Idle; }

			bool isIdle() { return state == STATE::Idle; }
			bool isPending() { return state == STATE::Pending; }
			bool isFinished() { return state == STATE::Finished; }
			bool isReady() { return state == STATE::Ready; }
		};

		class states
		{
			static const long MAX = 64L;
			pending children[MAX];

		public:
			states() { reset(); }

			void reset();

			bool isAnyIdle();
			bool isAnyReady();
			bool isAnyPending();
			bool isPending(long identity);
			bool isAnyFinished();

			bool setIdleToReady();
			bool setReadyToPending(long identity);

			bool setPendingToFinished(long identity);
			bool setFinishedToIdle(long identity);

			void output();
		};

		friend class listener;

		::server::listener *listen;

		mutex::token token;

		ERRORS lastErrorCode;
		bool isInError;

		bool init;

	public:
		states statuses;

	public:
		client() { makeNull(); reset(); }
		~client() { cleanup(); }

		bool initalised() { return init; }
		void reset();

		void clear();

		bool start();
		bool stopAndWait();
		bool pauseAndWait(bool value);
		bool isPaused();

		bool isError();
		void makeError(ERRORS code);
		void resetError();
		ERRORS lastError() { return lastErrorCode; }

		void shutdown();

	public:
		string identifier() { return string("server::client"); }

	protected:
		bool removeFromPending(long id);

	protected:
		void makeNull();
		void cleanup();
	};

	class server;

	class wait : public thread
	{
		server *s;

	public:
		DWORD WINAPI background(thread *bt);

	public:
		wait(server *source) { s = source; }
	};

	class watchdog : public thread
	{
		server *s;

	public:
		DWORD WINAPI background(thread *bt);

	public:
		watchdog(server *source) { s = source; }
	};

	class server : public ::wsock::server
	{
		friend class wait;
		friend class watchdog;

		client **clients;

		::server::configuration config;

		::server::wait *waiter;
		::server::watchdog *watcher;

		long counter, iterations;

		mutex::token token;

		bool init;

	public:
		server(::server::configuration &settings) { makeNull(); reset(settings); }
		~server() { cleanup(); }

		void reset(::server::configuration &settings);
		bool initalised() { return init; }

		bool open() { return ::wsock::server::open(config.port); }

		bool start();
		void stop();
		bool pauseAndWait(bool value);

		void shutdown();

	protected:
		bool finisherPauseAndWait(bool value);

	protected:
		client *findUnconnectedClient();
		client *findConnectedIdleClient(long &index);
		client *findConnectedReadyClient(long &index);
		client *findCompletedClient(long &index);

	protected:
		void output(string &source);

	protected:
		void makeNull();
		void cleanup();
	};
};

#endif