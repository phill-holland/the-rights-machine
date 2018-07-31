#include "response.h"
#include "error.h"
#include "parameters.h"

#if !defined(__OUTPUT)
#define __OUTPUT

namespace output
{
	class settings
	{
	public:
		string version;
		int status;
	};


	class output
	{
		string data;

		bool init;

//	public:
	//	int status;

	public:
		void clear()
		{
			data = string("");
		}

		bool isempty()
		{
			return ((long)data.size()) == 0L;
		}

		void set(data::json::response::json *source)
		{
			data = source->extract();
		}
		//void set(data::response::response &source)
		//{
		//}

		//void set(error::type::type &source)
		//{
			// convert to output type 
		//}

		string get()
		{
			//long size = (long)data.size();

			string result = "HTTP/1.1 200 OK\r\n";
			result += "Content-Length: " + string::fromLong((long)data.size() + 2L) + "\r\n";
			result += "Content-Type: application/json\r\n\r\n{";
			result += data;
			result += "}";// \r\n\r\n";

			return result;
			//web::parameters parameters;

			/*
			HTTP/1.1 200 OK
			Date: Mon, 27 Jul 2009 12:28:53 GMT
			Server: Apache/2.2.14 (Win32)
			Last-Modified: Wed, 22 Jul 2009 19:15:56 GMT
			Content-Length: 88
			Content-Type: text/html
			Connection: Closed
			*/
		}

	};
};

#endif