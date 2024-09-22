#include <cmath>
#include <string.h>
#include <unistd.h>
#include <sys/msg.h>

#include <not_implemented.h>

#include "../include/server_logger.h"

std::mutex server_logger::mutex;

server_logger::server_logger(std::map<std::string, std::set<severity>> const &log_dest):
    _configuration(log_dest)
{
    _mq_descriptor = msgget(LINUX_MSG_QUEUE_KEY, 0666);
	
	// if (_mq_descriptor == -1)
	// {
    //     throw std::runtime_error("Cannot connect to the server");
	// }
}

logger const *server_logger::log(
    const std::string &text,
    logger::severity severity) const noexcept
{
    for (auto record : _configuration)
    {
        const std::string &file_path = record.first;
        const auto &severities = record.second;
        
        if (severities.count(severity))
        {
            mutex.lock();
            
            msg_t msg;
            msg.mtype = LOG_PRIOR;
            msg.pid = getpid();
            msg.packet_cnt = std::ceil(1.0 * text.size() / MAX_MSG_TEXT_SIZE);
            msg.severity = static_cast<int>(severity);
            
            strcpy(msg.file_path, file_path.c_str());
            
            for (size_t i = 0; i < msg.packet_cnt; ++i)
            {
                msg.packet_id = i + 1;
                strcpy(msg.mtext, text.substr(i * MAX_MSG_TEXT_SIZE, MAX_MSG_TEXT_SIZE).c_str());
	            msgsnd(_mq_descriptor, &msg, sizeof(msg_t), 0);
            }
            
            mutex.unlock();
        }
    }
    
    return this;
}