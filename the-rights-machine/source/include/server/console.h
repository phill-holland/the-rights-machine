#include <stdio.h>
#include <iostream>
#include <fstream>
#include <chrono>
#include "core/string/string.h"
#include "core/threading/thread.h"
#include "core/threading/semaphore.h"
#include "core/queue/fifo.h"
#include "queues/interfaces/queue.h"
#include "server/error.h"

#ifndef _CONSOLE
#define _CONSOLE

namespace debug
{
    class console : public core::threading::thread, public ::queue::in<::error::type::type> 
    {
        const static long SECONDS = 10L;
        const static long QUEUE = 4096L;
        const static long BUFFER = 500L;

        core::queue::fifo<string, QUEUE> *data;

        std::chrono::high_resolution_clock::time_point clock;

        core::threading::semaphore::token token;

        int seconds;

        bool init;

    public:
        void background(thread *bt);

    public:
        console() { makeNull(); reset(); }
        ~console() { cleanup(); }

        void reset();
        bool initalised() { return init; }

        bool push(string source, bool timestamp = true);
        bool push(float source);
        bool push(long source);
        bool push(int source);
        bool push(bool value);

        bool set(::error::type::type &source)
        {
            string temp(source);
            return push(temp);
        }

    protected:
        void flush();

    protected:
        void floatToStr(char *temp, float number);
        void intToStr(char *temp, int number);
        void longToStr(char *temp, long number);
        void longHexToStr(char *temp, long number);
        void boolToStr(char *temp, bool value);

    protected:
        void makeNull();
        void cleanup();
    };
}

#endif
