#include "wsock.h"
#include "page.h"
#include "address.h"
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

			bool get(web::page *destination, web::page *source = NULL, web::parameter *parameters = NULL, long count = 0L);
			bool post(web::page *destination, web::page *source = NULL, web::parameter *parameters = NULL, long count = 0L);
			
			bool head(web::page *destination);			

		protected:
			bool issue(string &command, web::address &addr, web::page *destination, web::page *source, web::parameter *parameters, long count);
		};
	};
};

#endif