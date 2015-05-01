#ifndef __LOGGER__
#define __LOGGER__

#include <string>

class Logger
{
    Logger();
public:
    enum LogCategory {Info,Warning,Error,Critical};
    static void log(const std::string& msg, LogCategory category = Info);
};

#endif //__LOGGER__