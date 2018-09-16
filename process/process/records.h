#include "record.h"
#include "message.h"
#include "datetime.h"

namespace database
{
	namespace records
	{
		using namespace global;

		class message : public record::record<data::message::message>
		{
			static const long MAX = 1024L;

		public:
			long messageID;

			char user[MAX];
			char APIKey[MAX];
			char GUID[MAX];
			
		public:
			bool bind(database::recordset *recordset);
			void set(data::message::message &source);
		};
	};
};