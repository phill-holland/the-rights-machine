#include <stdio.h>
#include <iostream>
#include <fstream>
#include <chrono>
#include "string.h"
#include "thread.h"
#include "semaphore.h"
#include "fifo.h"
#include "queue.h"
#include "error.h"

#ifndef _CONSOLE
#define _CONSOLE

namespace debug
{
    class console : public thread, public ::queue::in<::error::type::type> 
    {
        const static long MINUTES = 2L;
        const static long QUEUE = 4096L;
        const static long BUFFER = 500L;

        fifo<string, QUEUE> *data;

        string filename;

        std::chrono::high_resolution_clock::time_point clock;

        semaphore::token token;

        int minutes;

        bool init;

    public:
        void background(thread *bt);

    public:
        console() { makeNull(); reset(string("debug.txt")); }
        console(string filename) { makeNull(); reset(filename); }
        ~console() { cleanup(); }

        void reset(string filename);
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
