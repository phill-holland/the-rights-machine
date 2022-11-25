#include "page.h"
#include "http.h"
#include "core/string/string.h"

namespace test
{
    class client
    {
        web::page source;
        http::client::client cli;

        long length;
        
        char *buffer;
        bool init;

    public:
        client(string filename) { makeNull(); reset(filename); }
        ~client() { cleanup(); }

        bool initalised() { return init; }
        void reset(string filename);

        bool post(string url, long port, web::page *destination);

    protected:
        void makeNull();
        void cleanup();
    };
};