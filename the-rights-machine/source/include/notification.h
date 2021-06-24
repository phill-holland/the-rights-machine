#include "guid.h"

#if !defined(__NOTIFICATION)
#define __NOTIFICATION

namespace notification
{
    class notification
    {
    public:
        virtual void notify_in(guid::guid identity) = 0;
        virtual void notify_out(guid::guid identity) = 0;
    };
};

#endif