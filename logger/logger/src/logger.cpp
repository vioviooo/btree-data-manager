#include "../include/logger.h"
#include <iomanip>
#include <sstream>

logger const *logger::trace(
    std::string const &message) const noexcept
{
    return log(message, logger::severity::trace);
}

logger const *logger::debug(
    std::string const &message) const noexcept
{
    return log(message, logger::severity::debug);
}

logger const *logger::information(
    std::string const &message) const noexcept
{
    return log(message, logger::severity::information);
}

logger const *logger::warning(
    std::string const &message) const noexcept
{
    return log(message, logger::severity::warning);
}

logger const *logger::error(
    std::string const &message) const noexcept
{
    return log(message, logger::severity::error);
}

logger const *logger::critical(
    std::string const &message) const noexcept
{
    return log(message, logger::severity::critical);
}

std::string logger::severity_to_string(
    logger::severity severity)
{
    switch (severity)
    {
        case logger::severity::trace:
            return "TRACE";
        case logger::severity::debug:
            return "DEBUG";
        case logger::severity::information:
            return "INFORMATION";
        case logger::severity::warning:
            return "WARNING";
        case logger::severity::error:
            return "ERROR";
        case logger::severity::critical:
            return "CRITICAL";
    }

    throw std::out_of_range("Invalid severity value");
}

logger::severity logger::string_to_severity(
        std::string str)
{
    for (size_t i = 0; i < str.size(); ++i)
    {
        str[i] = toupper(str[i]);
    }
    
    if (str == "TRACE")
        return severity::trace;
    else if (str == "DEBUG")
        return severity::debug;
    else if (str == "INFORMATION")
        return severity::information;
    else if (str == "WARNING")
        return severity::warning;
    else if (str == "ERROR")
        return severity::error;
    else if (str == "CRITICAL")
        return severity::critical;

    throw std::out_of_range("Invalid severity value");
}

std::string logger::global_datetime_to_string() noexcept
{
    auto time = std::time(nullptr);

    std::ostringstream result_stream;
    result_stream << std::put_time(std::gmtime(&time), "%d.%m.%Y %H:%M:%S");

    return result_stream.str();
}

std::string logger::current_datetime_to_string() noexcept
{
    auto time = std::time(nullptr);

    std::ostringstream result_stream;
    result_stream << std::put_time(std::localtime(&time), "%d.%m.%Y %H:%M:%S");

    return result_stream.str();
}