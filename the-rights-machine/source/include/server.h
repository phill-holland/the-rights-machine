#include "wsock.h"

#include "thread.h"
#include "message.h"
#include "fifo.h"
#include "manager.h"
#include "configuration.h"
#include "parameters.h"
#include "charbuf.h"
#include "pair.h"
#include "crumbs.h"
#include "responses.h"
#include "request.h"
#include "pending.h"
#include "error.h"
#include "output.h"

#if !defined(__SERVER)
#define __SERVER

namespace server
{
	class client;

	class listener : public thread
	{
		enum MODE { NONE = 0, POST = 1, GET = 2 };

		static const long RECEIVING = 16384L;
		static const long ERRORS = 5L;

		char receiving[RECEIVING];

		long errors;

		web::parameters parameters;
		crumbs::crumbs parents;

		bool quotes;
		bool left;

		long brackets, squares;

		bool header, request;// , validate; // turn into state int
		int h_index;

		charbuf command, label, value;

		compute::task task;
		::data::request::request requested;

		long content_length;
		long read_counter;

		output::output outputter;

		client *c;

		bool init;

	public:
		void background(thread *bt);

	public:
		listener(client *source) { reset(source); }

		bool initalised() { return init; }
		void reset(client *source);

	public:
		void clear();

	protected:
		MODE get();
		void validate();

	protected:
		void goodbye();
		void error(string error);
	};

	class client : public ::wsock::client
	{
		/*
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
					*/
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

		configuration::server::client::configuration configuration;

		mutex::token token;

		//ERRORS lastErrorCode;
		::error::error lastErrorCode;
		bool isInExit, isInError;

		bool init;

	public:
		states statuses;

	public:
		client(configuration::server::client::configuration &configuration) { makeNull(); reset(configuration); }
		~client() { cleanup(); }

		bool initalised() { return init; }
		void reset(configuration::server::client::configuration &configuration);

		void clear();

		bool start();
		bool stopAndWait();
		bool pauseAndWait(bool value);
		bool isPaused();

		void goodbye();
		bool isDeparting();
		void resetExit();

		bool isError();
		void makeError(::error::error &error);
		//void makeError(ERRORS code);
		void resetError();
		//ERRORS lastError() { return lastErrorCode; }
		::error::error lastError() { return lastErrorCode; }

		void shutdown();

	public:
		string identifier() { return string("server::client"); }

	//protected:
	//	bool removeFromPending(long id);

	protected:
		void makeNull();
		void cleanup();
	};

	class server;

	class wait : public thread
	{
		server *s;

	public:
		void background(thread *bt);

	public:
		wait(server *source) { s = source; }
	};

	class watchdog : public thread
	{
		server *s;

	public:
		void background(thread *bt);

	public:
		watchdog(server *source) { s = source; }
	};

	class server : public ::wsock::server
	{
		friend class wait;
		friend class watchdog;

		::server::client **clients;

		::configuration::server::configuration configuration;

		::pending::pending *requested;

		::server::wait *waiter;
		::server::watchdog *watcher;

		long counter, iterations;

		mutex::token token;

		bool init;

	public:
		server(configuration::server::configuration *settings) { makeNull(); reset(settings); }
		~server() { cleanup(); }

		void reset(configuration::server::configuration *settings);
		bool initalised() { return init; }

		bool open() { return ::wsock::server::open(configuration.port); }

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
		void output(string source);

	protected:
		void makeNull();
		void cleanup();
	};
};

#endif