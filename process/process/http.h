#include "wsock.h"
#include "page.h"
#include "ssl.h"

#if !defined(__HTTP)
#define __HTTP

namespace http
{
	namespace client
	{
		class client : public ::wsock::client, public ssl
		{
			static char CRLF[3];

			static const long receive_length = 32768L;
			char receive[receive_length];

			long status;

		public:
			client() : ::wsock::client(), ssl() { reset(); }

			void reset()
			{
				status = 0L;
			}

			bool get(web::page &destination);
			bool get(web::page &destination, web::parameter *parameters, long count);
			bool head(web::page &destination);
			// put post here!!

		protected:
			bool issue(string &command, web::address &addr, web::page &destination, web::parameter *parameters, long count);
		};
	};
	/*
	namespace server
	{
		class client : public ::wsock::client
		{
		};
		
		class server : public ::wsock::server
		{
			client **clients;

		public:

		};
	};*/
};

#endif