#include "Logger.h"
#include <iostream>

Logger::Logger()
{
}

void Logger::log(const std::string& msg, LogCategory category)
{
    std::cout << msg << std::endl;
}