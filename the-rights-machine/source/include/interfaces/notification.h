#include "core/custom/guid.h"

#if !defined(_NOTIFICATION)
#define _NOTIFICATION

namespace notification
{
    class notification
    {
    public:
        virtual void notifyIn(core::custom::guid identity) = 0;
        virtual void notifyOut(core::custom::guid identity) = 0;
    };
};

#endif