#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <thread>
#include <sys/msg.h>

#include <file_cannot_be_opened.h>

#define LINUX_MSG_QUEUE_KEY 100
#define WIN32_MAILSLOT_NAME "\\\\.\\mailslot\\os_cw_srvr_lgr"
#define MAX_MSG_TEXT_SIZE 1024
#define SHUTDOWN_PRIOR 1
#define LOG_PRIOR 2

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

int run_flag = 1;
int mq_descriptor = -1;


std::string decode_severity(int severity_code);
void terminal_reader();


int main()
{
    msg_t msg;
    size_t const max_msg_size = sizeof(msg_t);
    std::map<std::string, std::ofstream> streams;
    
    mq_descriptor = msgget(LINUX_MSG_QUEUE_KEY, IPC_CREAT | 0666);
    if (mq_descriptor == -1)
    {
        std::cout << "Cannot create the queue. Shut down." << std::endl;
        return 1;
    }
    
    std::thread cmd_thread(terminal_reader);
    
    while (run_flag)
    {
        ssize_t rcv_cnt = msgrcv(mq_descriptor, &msg, max_msg_size, 0, MSG_NOERROR);
        if (rcv_cnt == -1)
        {
            std::cout << "An error occured while receiving the message" << std::endl;
            
            break;
        }
        
        if (msg.mtype == SHUTDOWN_PRIOR)
        {
            break;
        }
        
        if (msg.file_path[0] && !streams.count(msg.file_path))
        {	
            std::ofstream fstream = std::ofstream(msg.file_path, std::ios::app);
            
            if (!fstream.is_open())
            {
                std::cerr << file_cannot_be_opened(msg.file_path).what() << std::endl;
            }
            
            streams[msg.file_path] = std::move(fstream);
        }
        
        if (msg.packet_cnt == 1)
        {
            std::ostream *log_stream_ptr = &std::cout;
            if (msg.file_path[0])
            {
                log_stream_ptr = &streams[msg.file_path];
            }
            
            std::ostream &log_stream = *log_stream_ptr;
            std::string severity = decode_severity(msg.severity);
            
            log_stream << "[" << severity << "] " << msg.mtext << std::endl;
            log_stream.flush();
        }
        else
        {
            std::string tmp_file = "tmp_file_" + std::to_string(msg.pid);
            
            std::ofstream &stream = streams[tmp_file];
            
            if (msg.packet_id == 1)
            {
                stream.close();
                stream.open(tmp_file, std::ios::out | std::ios::trunc);
            }
            
            stream << msg.mtext;
            
            if (msg.packet_id == msg.packet_cnt)
            {
                stream.flush();
                
                std::ifstream istream(tmp_file);
                
                if (istream.is_open())
                {
                    std::ostream *log_stream_ptr = &std::cout;
                    if (msg.file_path[0])
                    {
                        log_stream_ptr = &streams[msg.file_path];
                    }
                    
                    std::ostream &log_stream = *log_stream_ptr;
                    std::string severity = decode_severity(msg.severity);
                    
                    log_stream << "[" << severity << "] ";
                    
                    char ch;
                    while (istream >> std::noskipws >> ch)
                    {
                        log_stream << ch;
                    }
                    
                    log_stream << std::endl;
                    log_stream.flush();
                    
                    istream.close();
                }
            }
        }
    }
    
    for (auto &record : streams)
    {
        std::ofstream &stream = record.second;
        stream.flush();
        stream.close();
    }
    
    msgctl(mq_descriptor, IPC_RMID, nullptr);
    
    std::cout << "Server shut down" << std::endl;
    
    cmd_thread.detach();
}



std::string decode_severity(int severity_code)
{
    switch (severity_code)
    {
    case 0:
        return "TRACE";
    case 1:
        return "DEBUG";
    case 2:
        return "INFORMATION";
    case 3:
        return "WARNING";
    case 4:
        return "ERROR";
    case 5:
        return "CRITICAL";
    default:
        return "UNKNOWN";
    }
}

void terminal_reader()
{
    msg_t msg;
    std::string cmd;
    
    while (std::cin >> cmd)
    {
        if (cmd == "shutdown")
        {
            msg.mtype = SHUTDOWN_PRIOR;
            msgsnd(mq_descriptor, &msg, sizeof(msg_t), 0);
            break;
        }
    }
}