#include "net/web/page.h"
#include "net/web/client.h"
#include "core/string/string.h"

namespace test
{
    class client
    {
        net::web::page source;
        net::web::client cli;

        long length;
        
        char *buffer;
        bool init;

    public:
        client(string filename) { makeNull(); reset(filename); }
        ~client() { cleanup(); }

        bool initalised() { return init; }
        void reset(string filename);

        bool post(string url, long port, net::web::page *destination);

    protected:
        void makeNull();
        void cleanup();
    };
};