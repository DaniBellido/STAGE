#include "Logger.h"
#include <iostream>
#include <ctime>
#include <array>
#include <iomanip>  // For setw and setfill
#include <sstream> // for std::stringstream

#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"

std::vector<LogEntry> Logger::messages;

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
    std::string newMessage = "[LOG]: " + getTime() + " - " + message;

    LogEntry logEntry;
    logEntry.type = LOG_INFO;
    logEntry.message = newMessage;

    std::cout << GREEN << logEntry.message << RESET << std::endl;

    messages.push_back(logEntry);
}

void Logger::Err(const std::string& message)
{
    std::string newMessage = "[ERR]: " + getTime() + " - " + message;

    LogEntry logEntry;
    logEntry.type = LOG_ERROR;
    logEntry.message = newMessage;

    std::cerr << RED << logEntry.message << RESET << std::endl;

    messages.push_back(logEntry);

}
