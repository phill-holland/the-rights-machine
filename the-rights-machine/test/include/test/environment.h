#include <gtest/gtest.h>
#include "messaging.h"
#include "settings.h"
#include "starter.h"
#include "page.h"

namespace test
{
    class TestEnvironment : public ::testing::Environment
    {
        messaging::memory::memory *messaging;
        server::settings *setup;
        server::starter *starter;

        const static long port = 5454;

    public:
        TestEnvironment()
        {
            messaging = NULL;
            setup = NULL;
            starter = NULL;		
        }

        virtual void SetUp() 
        {
            messaging = new messaging::memory::memory();
            if(messaging == NULL) return;

            setup = new server::settings(messaging, port);
            if(setup == NULL) return;

            starter = new server::starter(*setup);
            if(starter == NULL) return;

            if(starter->initalised())
                starter->start();
        }

        virtual void TearDown()
        {
            if(starter != NULL) delete starter;
            if(setup != NULL) delete setup;
            if(messaging != NULL) delete messaging;
        }
    };
};