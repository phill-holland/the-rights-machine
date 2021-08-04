#include "parser.h"
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
#include "notification.h"

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

		bool left;

		bool header, request;
		int h_index;

		charbuf command, label, value;

		::data::request::request requested;

		long content_length;
		long read_counter;

		parser::parser *parser;

		client *c;

		boost::json::error_code ec;

		bool init;

	public:
		void background(thread *bt);

	public:
		listener(client *source) { makeNull(); reset(source); }
		~listener() { cleanup(); }

		bool initalised() { return init; }
		void reset(client *source);

	public:
		void clear();

	protected:
		MODE get();
		bool validate();
		bool intent(char *source, int length, int &index);
		bool heading(char *source, int length, int &index);

	protected:
		void goodbye();
		void error(string error);

	protected:
		void makeNull() { parser = NULL; }
		void cleanup() { if (parser != NULL) delete parser; }
	};

	class client : public ::wsock::client, notification::notification
	{
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

			string output();
		};

		friend class listener;

		::server::listener *listen;

		configuration::server::client::configuration configuration;

		mutex::token token;

		::error::error lastErrorCode;
		bool isInExit, isInError;

		int in, out;
		
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
		void resetError();
		::error::error lastError() { return lastErrorCode; }

		void shutdown();

		string identifier() { return string("server::client"); }

	public:
		void notifyIn(guid::guid identity);
		void notifyOut(guid::guid identity);

	protected:
		bool startResponses();
		bool endResponses();
		bool sendResponse(data::response::response response);

	protected:
		void output(error::error source);
		void output(string source);

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