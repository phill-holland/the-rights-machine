#include "queues/interfaces/queue.h"
#include "compute/manager.h"
#include "server/errors.h"
#include "server/pending.h"
#include "database/users.h"

#if !defined(__CONFIGURATION)
#define __CONFIGURATION

namespace server
{
	class server;
	class client;
	class listener;
};

namespace configuration
{
	namespace server
	{
		namespace client
		{
			class configuration;
		};

		class configuration
		{
			friend class ::server::server;
			friend class ::configuration::server::client::configuration;

			manager::manager *manager;
			data::users *users;
			error::errors *errors;

		public:
			long port;
			long clients;

		protected:
			const static long PORT = 5555L;
			const static long CLIENTS = 1L;

		protected:
			configuration() { reset(NULL, NULL, NULL); }

		public:
			configuration(manager::manager *manager, data::users *users, error::errors *errors) { reset(manager, users, errors); }
			configuration(configuration const &source) { copy(source); }

			void reset(manager::manager *manager, data::users *users, error::errors *errors);

			void copy(configuration const &source);

		public:
			configuration& operator=(const configuration& source)
			{
				this->copy((configuration&)source);
				return *this;
			}
		};

		namespace client
		{
			class configuration
			{
				friend class ::server::client;
				friend class ::server::listener;

				manager::manager *manager;
				custom::chain<models::response::response> *responses;
				pending::pending *requested;
				data::users *users;

				error::errors *errors;

			protected:
				configuration() { reset(NULL, NULL, NULL, NULL, NULL); }

			public:
				configuration(manager::manager *manager,
							  custom::chain<models::response::response> *responses,
							  pending::pending *requested,
							  data::users *users,
							  error::errors *errors)
				{
					reset(manager, responses, requested, users, errors);
				}
				configuration(configuration const &source) { copy(source); }
				configuration(::configuration::server::configuration const &source, pending::pending *requested = NULL) { copy(source, requested); }

				void reset(manager::manager *manager,
						   custom::chain<models::response::response> *responses,
						   pending::pending *requested,
						   data::users *users,
						   error::errors *errors);

				void copy(configuration const &source);
				void copy(::configuration::server::configuration const &source, pending::pending *requested = NULL);

			public:
				configuration& operator=(const configuration& source)
				{
					this->copy((configuration&)source);
					return *this;
				}

				configuration& operator=(const ::configuration::server::configuration& source)
				{
					this->copy((::configuration::server::configuration&)source);
					return *this;
				}
			};
		};
	};
};

#endif