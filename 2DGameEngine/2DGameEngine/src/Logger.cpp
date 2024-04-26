#include "Logger.h"
#include <iostream>
#include <ctime>
#include <array>
#include <iomanip>  // For setw and setfill
#include <sstream> // for std::stringstream

#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"

std::string getTime()
{
    std::time_t now = std::time(nullptr);

    std::tm time;
    localtime_s(&time, &now);

    // Array to store month names
    std::array<std::string, 12> monthNames = {
        "Jan", "Feb", "Mar", "Apr", "May", "Jun",
        "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
    };

    std::stringstream ss;
    ss << std::setw(2) << std::setfill('0') << time.tm_mday << "/"     // Day of the month
        << monthNames[time.tm_mon] << "/"                              // Month
        << std::setw(4) << time.tm_year + 1900 << " "                  // Year (since 1900)
        << std::setw(2) << std::setfill('0') << time.tm_hour << ":"    // Hour
        << std::setw(2) << std::setfill('0') << time.tm_min << ":"     // Minute
        << std::setw(2) << std::setfill('0') << time.tm_sec;           // Second

    return ss.str();
}

void Logger::Log(const std::string& message)
{
    std::cout << GREEN << "[LOG]: " << getTime() << " - " << message << RESET << std::endl;
}

void Logger::Err(const std::string& message)
{
    std::cerr << RED << "[ERR]: " << getTime() << " - " << message << RESET << std::endl;
}
