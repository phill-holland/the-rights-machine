#include "types/guid.h"

#if !defined(_NOTIFICATION)
#define _NOTIFICATION

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