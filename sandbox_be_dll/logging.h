#pragma once
#include "colorprintf.h"
#include "includes.h"

#define LOG_FILE "C:\\Users\\user123\\Desktop\\testing_drivers\\test_logs.txt"
#define LOG_MAX_LEN 256

/*  Singleton for the Logger    */

class Logger
{
private:
    static Logger* logger;

    Logger();

public:

    static Logger* Get()
    {
        if (!logger)
        {
            logger = new Logger();
        }

        return logger;
    }

    void Print(COLOR_ID color, const char* format, ...)
    {
        static char buffer[256];
        va_list args;
        va_start(args, format);
        vsprintf(buffer, format, args);
        va_end(args);
        colorprintf(color, buffer);
    }
};
