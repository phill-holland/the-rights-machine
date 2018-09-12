#include "response.h"
#include "error.h"
#include "parameters.h"
#include "log.h"

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

		string get()
		{
			string result = "HTTP/1.1 200 OK\r\n";
			result += "Content-Length: " + string::fromLong((long)data.size() + 2L) + "\r\n";
			result += "Content-Type: application/json\r\n\r\n{";
			result += data;
			result += "}";

			return result;
		}

	};
};

#endif