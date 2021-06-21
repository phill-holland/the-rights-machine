#include "custom/string.h"
#include "messaging.h"

#if !defined(__SETTINGS)
#define __SETTINGS

namespace server
{
    class settings
    {
    public:
        long port;
        long clients;
        string connection;

        messaging::common::messaging *messaging;

    protected:
        const static long PORT = 5555L;
        const static long CLIENTS = 1L;

    public:
        settings (messaging::common::messaging *messaging, long port = PORT, long clients = CLIENTS, string connection = "")
        {
            this->messaging = messaging;
            this->port = port;
            this->clients = clients;
            this->connection = connection;
        }
        settings(const settings &source) { copy(source); }

    public:
        void copy(settings const &source)
        {
            messaging = source.messaging;
            port = source.port;
            clients = source.clients;
            connection = source.connection;
        }

    public:
        settings& operator=(const settings& source)
        {
            this->copy((settings&)source);
            return *this;
        }
    };
};

#endif