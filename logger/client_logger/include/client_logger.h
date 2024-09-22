#ifndef MATH_PRACTICE_AND_OPERATING_SYSTEMS_CLIENT_LOGGER_H
#define MATH_PRACTICE_AND_OPERATING_SYSTEMS_CLIENT_LOGGER_H

#include <logger.h>
#include "client_logger_builder.h"

#include <map>
#include <unordered_map>
#include <set>

class client_logger final:
    public logger
{

    friend class client_logger_builder;

private:

    static std::unordered_map<std::string, std::pair<std::ostream *, size_t>> _all_streams;

private:

    std::string _format_string;
    std::map<std::string, std::pair<std::ostream *, std::set<logger::severity>>> _streams;

private:

    client_logger(
        std::string const &format_string,
        std::map<std::string, std::set<logger::severity>> const &configuration);

public:

    client_logger(
        client_logger const &other);

    client_logger &operator=(
        client_logger const &other);

    client_logger(
        client_logger &&other) noexcept;

    client_logger &operator=(
        client_logger &&other) noexcept;

    ~client_logger() noexcept final;

public:

    [[nodiscard]] logger const *log(
        std::string const &message,
        logger::severity severity) const noexcept override;

private:
    
    void decrement_stream_users(std::string const &file_path) const noexcept;

};

#endif //MATH_PRACTICE_AND_OPERATING_SYSTEMS_CLIENT_LOGGER_H