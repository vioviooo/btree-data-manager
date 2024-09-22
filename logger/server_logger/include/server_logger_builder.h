#ifndef MATH_PRACTICE_AND_OPERATING_SYSTEMS_SERVER_LOGGER_BUILDER_H
#define MATH_PRACTICE_AND_OPERATING_SYSTEMS_SERVER_LOGGER_BUILDER_H

#include <logger_builder.h>

#include <set>
#include <map>

class server_logger_builder final:
    public logger_builder
{

private:

    std::map<std::string, std::set<logger::severity>> _configuration;

public:

    server_logger_builder() = default;

    server_logger_builder(
        server_logger_builder const &other) = default;

    server_logger_builder &operator=(
        server_logger_builder const &other) = default;

    server_logger_builder(
        server_logger_builder &&other) noexcept = default;

    server_logger_builder &operator=(
        server_logger_builder &&other) noexcept = default;

    ~server_logger_builder() noexcept override = default;

public:

    logger_builder *add_file_stream(
        std::string const &stream_file_path,
        logger::severity severity) override;

    logger_builder *add_console_stream(
        logger::severity severity) override;

    logger_builder* transform_with_configuration(
        std::string const &configuration_file_path,
        std::string const &configuration_path) override;

    logger_builder *clear() override;

    [[nodiscard]] logger *build() const override;

};

#endif //MATH_PRACTICE_AND_OPERATING_SYSTEMS_SERVER_LOGGER_BUILDER_H