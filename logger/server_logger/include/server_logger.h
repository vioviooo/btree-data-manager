#ifndef MATH_PRACTICE_AND_OPERATING_SYSTEMS_SERVER_LOGGER_H
#define MATH_PRACTICE_AND_OPERATING_SYSTEMS_SERVER_LOGGER_H

#include <logger.h>
#include "server_logger_builder.h"

#ifdef _WIN32
#include <windows.h>
#endif

#include <set>
#include <map>
#include <mutex>

#define LINUX_MSG_QUEUE_KEY 100
#define WIN32_MAILSLOT_NAME "\\\\.\\mailslot\\os_cw_srvr_lgr"
#define MAX_MSG_TEXT_SIZE 1024
#define LOG_PRIOR 2

class server_logger final:
    public logger
{

    friend class server_logger_builder;

private:
    struct msg_t
    {
        long mtype;
        pid_t pid;
        size_t packet_id;
        size_t packet_cnt;
        char file_path[256];
        int severity;
        char mtext[MAX_MSG_TEXT_SIZE];
    };

private:

    static std::mutex mutex;

private:

    #ifdef __APPLE__
    int _mq_descriptor;
    #endif
    
    #ifdef _WIN32
    HANDLE _hFile;
    #endif
    
    std::map<std::string, std::set<severity>> _configuration;

private:

    server_logger(
        std::map<std::string, std::set<severity>> const &configuration);

public:

    server_logger(
        server_logger const &other) = default;

    server_logger &operator=(
        server_logger const &other) = default;

    server_logger(
        server_logger &&other) noexcept = default;

    server_logger &operator=(
        server_logger &&other) noexcept = default;

    ~server_logger() noexcept final = default;

public:

    [[nodiscard]] logger const *log(
        const std::string &message,
        logger::severity severity) const noexcept override;

};

#endif //MATH_PRACTICE_AND_OPERATING_SYSTEMS_SERVER_LOGGER_H