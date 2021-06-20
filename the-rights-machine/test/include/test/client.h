#include "page.h"
#include "http.h"
#include "custom/string.h"

namespace test
{
    class client
    {
        web::page page;
        http::client::client cli;

        char *buffer;
        bool init;

    public:
        client(string filename) { makeNull(); reset(filename); }
        ~client() { cleanup(); }

        bool initalised() { return init; }
        void reset(string filename);

        bool post(string url, long port);

    protected:
        void makeNull();
        void cleanup();
    };
};