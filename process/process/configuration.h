#include "queue.h"
#include "manager.h"
#include "errors.h"

#if !defined(__CONFIGURATION)
#define __CONFIGURATION

namespace server
{
	class server;

	namespace configuration
	{
		/*
		class settings
		{
			friend class server;

		public:
			long port;
			long clients;

		protected:
			const static long PORT = 5555L;
			const static long CLIENTS = 1L;

		public:
			manager::manager *manager;
			error::errors *errors;

		public:
			settings() 
			{ 
				manager = NULL; 
				errors = NULL; 

				port = PORT;
				clients = CLIENTS;
			}

			settings(settings const &source) { copy(source); }

			void copy(settings const &source)
			{
				manager = source.manager;
				errors = source.errors;
				port = source.port;
				clients = source.clients;
			}

		public:
			settings& operator=(const settings& source)
			{
				this->copy((settings&)source);
				return *this;
			}
		};
		*/
		class configuration
		{
			friend class server;

			//settings settings;
			manager::manager *manager;
			error::errors *errors;

		public:
			long port;
			long clients;

		protected:
			const static long PORT = 5555L;
			const static long CLIENTS = 1L;

		protected:
			configuration() { reset(NULL, NULL); }

		public:
			configuration(manager::manager *manager, error::errors *errors) { reset(manager, errors); }		
			configuration(configuration const &source) { copy(source); }

			void reset(manager::manager *manager, error::errors *errors);

			//manager::manager *get() { return settings.manager; }

			void copy(configuration const &source);

		public:
			configuration& operator=(const configuration& source)
			{
				this->copy((configuration&)source);
				return *this;
			}
		};
	};
};

#endif