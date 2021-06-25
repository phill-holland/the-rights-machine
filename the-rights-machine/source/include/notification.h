#include "guid.h"

#if !defined(__NOTIFICATION)
#define __NOTIFICATION

namespace notification
{
    class notification
    {
    public:
        virtual void notifyIn(guid::guid identity) = 0;
        virtual void notifyOut(guid::guid identity) = 0;
    };
};

#endif