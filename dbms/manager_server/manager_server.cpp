#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <string>
#include <cstring>
#include <vector>
#include <map>
#include <unordered_map>
#include <thread>
#include <mutex>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/msg.h>
#include <sys/wait.h>

#include <extra_utility.h>
#include <ipc_data.h>
#include <tdata.h>
#include <b_tree.h>
#include <csignal>


struct db_path
{

public:

    std::string pool, schema, collection;

    db_path(
            std::string const &p,
            std::string const &s,
            std::string const &c):
            pool(p),
            schema(s),
            collection(c)
    { }

    friend bool operator<(
            db_path const &lhs,
            db_path const &rhs)
    {
        if (lhs.pool != rhs.pool) return lhs.pool < rhs.pool;
        if (lhs.schema != rhs.schema) return lhs.schema < rhs.schema;
        return lhs.collection < rhs.collection;
    }
};

template<>
struct std::hash<db_path>
{
    std::size_t operator()(const db_path& s) const noexcept
    {
        return std::hash<std::string>{}(s.pool + s.schema + s.collection);
    }
};



int run_flag = 1;
int mq_descriptor = -1;



void load_config(
        size_t &strg_servers_cnt);

void save_config(
        size_t strg_servers_cnt);

void load_separators(
        std::map<db_path, std::vector<std::string>> &separators,
        size_t strg_servers_cnt);

void run_terminal_reader(
        std::vector<pid_t> *strg_servers);

void run_assistant_reader();

pid_t run_storage_server(
        size_t strg_server_id);


size_t get_id_for_key(
        std::map<db_path, std::vector<std::string>> const &separators,
        db_path path,
        std::string const &key);

int redistribute_keys(
        std::vector<pid_t> const &strg_servers,
        std::map<db_path, std::vector<std::string>> &separators);


void handle_add_server_command(
        bool is_filesystem,
        std::vector<pid_t> &strg_servers,
        std::map<db_path, std::vector<std::string>> &separators,
        db_ipc::strg_msg_t &msg);

void handle_terminate_server_command(
        std::vector<pid_t> &strg_servers,
        std::map<db_path, std::vector<std::string>> &separators,
        db_ipc::strg_msg_t &msg);

void handle_add_struct_command(
        std::vector<pid_t> const &strg_servers,
        std::map<db_path, std::vector<std::string>> &separators,
        db_ipc::strg_msg_t &msg);

void handle_dispose_struct_command(
        std::vector<pid_t> const &strg_servers,
        std::map<db_path, std::vector<std::string>> &separators,
        db_ipc::strg_msg_t &msg);

void handle_data_command(
        std::vector<pid_t>const &strg_servers,
        std::map<db_path, std::vector<std::string>> const &separators,
        db_ipc::strg_msg_t &msg);

void handle_obtain_between_command(
        std::vector<pid_t> const &strg_servers,
        std::map<db_path, std::vector<std::string>> const &separators,
        db_ipc::strg_msg_t &msg);



int main()
{
    msgctl(msgget(db_ipc::MANAGER_SERVER_MQ_KEY, 0666), IPC_RMID, nullptr);

    bool is_filesystem = false;
    size_t data_change_counter = 0;

    db_ipc::strg_msg_t msg;
    size_t initial_strg_servers_cnt;
    std::vector<pid_t> strg_servers;
    std::map<db_path, std::vector<std::string>> separators;

    if (is_filesystem)
    {
        try
        {
            load_config(initial_strg_servers_cnt);
            load_separators(separators, initial_strg_servers_cnt);
        }
        catch (std::ios::failure const &)
        {
            std::cout << "Cannot load database" << std::endl;
            return 1;
        }
        catch (std::runtime_error const &ex)
        {
            std::cout << ex.what() << std::endl;
            std::cout << "Servers count set to 1" << std::endl;
            initial_strg_servers_cnt = 1;
        }
    }
    else
    {
        initial_strg_servers_cnt = 1;
    }

    std::thread cmd_thread(run_terminal_reader, &strg_servers);

    mq_descriptor = msgget(db_ipc::MANAGER_SERVER_MQ_KEY, IPC_CREAT | 0666);
    if (mq_descriptor == -1)
    {
        std::cout << "Cannot create the queue. Shut down." << std::endl;
        return 3;
    }

    int rcv = -1;
    do
    {
        rcv = msgrcv(mq_descriptor, &msg, db_ipc::MANAGER_SERVER_MSG_SIZE, 0, IPC_NOWAIT);
    } while (rcv != -1);

    msg.mtype = 1;
    msg.cmd = db_ipc::command::ADD_STORAGE_SERVER;
    for (size_t i = 0; i < initial_strg_servers_cnt; ++i)
    {
        msg.extra_value = i;
        msgsnd(mq_descriptor, &msg, db_ipc::MANAGER_SERVER_MSG_SIZE, MSG_NOERROR);
    }

    while (run_flag)
    {
        ssize_t rcv_cnt = msgrcv(mq_descriptor, &msg, db_ipc::MANAGER_SERVER_MSG_SIZE, -db_ipc::MANAGER_SERVER_MAX_COMMON_PRIOR, MSG_NOERROR);

        if (rcv_cnt == -1)
        {
            std::cout << "An error occured while receiving the message" << std::endl;
            break;
        }

        std::cout << "MNGR (" << getpid() << ") read from " << msg.pid << std::endl;

        msg.mtype = msg.pid;

        switch (msg.cmd)
        {
            case db_ipc::command::PING:
            {
                msg.mtype = msg.pid;
                msg.status = db_ipc::command_status::OK;
            }
            case db_ipc::command::SHUTDOWN:
            {
                try
                {
                    save_config(strg_servers.size());
                }
                catch (std::ios::failure const &ex)
                {
                    std::cout << ex.what() << std::endl;
                }

                run_flag = 0;
                break;
            }
            case db_ipc::command::ADD_STORAGE_SERVER:
            {
                try
                {
                    handle_add_server_command(is_filesystem, strg_servers, separators, msg);
                    save_config(strg_servers.size());
                }
                catch (std::ios::failure const &ex)
                {
                    std::cout << ex.what() << std::endl;
                }
                catch (std::runtime_error &ex)
                {
                    std::cout << ex.what() << std::endl;
                }
                break;
            }
            case db_ipc::command::TERMINATE_STORAGE_SERVER:
            {
                try
                {
                    handle_terminate_server_command(strg_servers, separators, msg);
                    save_config(strg_servers.size());
                }
                catch (std::ios::failure const &ex)
                {
                    std::cout << ex.what() << std::endl;
                }
                catch (std::runtime_error &ex)
                {
                    std::cout << ex.what() << std::endl;
                }

                break;
            }
            case db_ipc::command::ADD_POOL:
            case db_ipc::command::ADD_SCHEMA:
            case db_ipc::command::ADD_COLLECTION:
            {
                handle_add_struct_command(strg_servers, separators, msg);
                break;
            }
            case db_ipc::command::DISPOSE_POOL:
            case db_ipc::command::DISPOSE_SCHEMA:
            case db_ipc::command::DISPOSE_COLLECTION:
            {
                handle_dispose_struct_command(strg_servers, separators, msg);
                break;
            }
            case db_ipc::command::ADD:
            case db_ipc::command::UPDATE:
            case db_ipc::command::DISPOSE:
            {
                ++data_change_counter;
                handle_data_command(strg_servers, separators, msg);
                break;
            }
            case db_ipc::command::OBTAIN:
            case db_ipc::command::OBTAIN_MIN:
            case db_ipc::command::OBTAIN_MAX:
            case db_ipc::command::OBTAIN_NEXT:
            {
                handle_data_command(strg_servers, separators, msg);
                break;
            }
            case db_ipc::command::OBTAIN_BETWEEN:
            {
                handle_obtain_between_command(strg_servers, separators, msg);
            }
            default:
                break;
        }

        if (data_change_counter >= 30 * (1 + strg_servers.size()))
        {
            redistribute_keys(strg_servers, separators);
            data_change_counter = 0;
        }
    }

    msgctl(mq_descriptor, IPC_RMID, nullptr);

    std::cout << "Server shutdowned" << std::endl;

    cmd_thread.detach();
}

void load_config(
        size_t &strg_servers_cnt)
{
    std::ifstream stream("cfg");
    if(!stream.is_open())
    {
        strg_servers_cnt = 1;
        return;
    }

    stream >> strg_servers_cnt;

    if (stream.fail())
    {
        throw std::ios::failure("Failed to read configuration file");
    }
    if (strg_servers_cnt <= 0)
    {
        throw std::runtime_error("Invalid configuration file");
    }
}

void save_config(
        size_t strg_servers_cnt)
{
    std::ofstream stream("cfg");
    stream << strg_servers_cnt;

    if (stream.fail())
    {
        throw std::ios::failure("Failed to save configuration file");
    }
}

void load_separators(
        std::map<db_path, std::vector<std::string>> &separators,
        size_t strg_servers_cnt)
{
    if (access("pools", F_OK) == -1) return;

    for (auto const &pool_entry : std::filesystem::directory_iterator("pools"))
    {
        if (!std::filesystem::is_directory(pool_entry)) continue;

        std::string pool_name = pool_entry.path().filename();
        std::string pool_cfg_path = extra_utility::make_path({"pools", pool_name, "cfg"});

        if (access(pool_cfg_path.c_str(), F_OK) == -1) continue;

        for (auto const &schema_entry : std::filesystem::directory_iterator(extra_utility::make_path({"pools", pool_name})))
        {
            if (!std::filesystem::is_directory(schema_entry)) continue;

            std::string schema_name = schema_entry.path().filename();
            std::string schema_cfg_path = extra_utility::make_path({"pools", pool_name, schema_name, "cfg"});

            if (access(schema_cfg_path.c_str(), F_OK) == -1) continue;

            for (auto const &collection_entry : std::filesystem::directory_iterator(extra_utility::make_path({"pools", pool_name, schema_name})))
            {
                if (!std::filesystem::is_directory(collection_entry)) continue;

                std::string collection_name = collection_entry.path().filename();
                std::string collection_cfg_path = extra_utility::make_path({"pools", pool_name, schema_name, collection_name, "cfg"});

                if (access(collection_cfg_path.c_str(), F_OK) == -1) continue;

                separators[db_path(pool_name, schema_name, collection_name)] =
                        std::vector<std::string>(strg_servers_cnt - 1, std::string(db_ipc::MSG_KEY_SIZE, 127));

                for (auto const &table_entry : std::filesystem::directory_iterator(extra_utility::make_path({"pools", pool_name, schema_name, collection_name})))
                {
                    if (std::filesystem::is_directory(table_entry)) continue;

                    std::string file_name = table_entry.path().filename();
                    size_t id = 0;

                    for (auto ch : file_name)
                    {
                        if (!isdigit(ch))
                        {
                            id = 0; break;
                        }
                        id *= 10;
                        id += ch - '0';
                    }

                    if (id <= 1 || id > strg_servers_cnt) continue;

                    std::string data_path = extra_utility::make_path({"pools", pool_name, schema_name, collection_name, table_entry.path().filename()});
                    std::ifstream data_stream(data_path);

                    if (data_stream.peek() != EOF)
                    {
                        char ch;
                        size_t login_len;
                        std::string login;

                        data_stream.read(reinterpret_cast<char *>(&login_len), sizeof(size_t));
                        login.reserve(login_len);
                        for (size_t i = 0; i < login_len; ++i)
                        {
                            data_stream.read(&ch, sizeof(char));
                            login.push_back(ch);
                        }

                        separators[db_path(pool_name, schema_name, collection_name)][id - 2] = login;
                    }
                }
            }
        }
    }
}

void run_terminal_reader(
        std::vector<pid_t> *strg_servers)
{
    db_ipc::strg_msg_t msg;
    std::string line, cmd;

    while (std::getline(std::cin, cmd))
    {
        if (cmd == "shutdown")
        {
            run_flag = 0;
            msg.cmd = db_ipc::command::SHUTDOWN;
            msg.pid = getpid();

            for (auto iter = strg_servers->begin(); iter != strg_servers->end(); ++iter)
            {
                msg.mtype = *iter;
                msgsnd(mq_descriptor, &msg, db_ipc::MANAGER_SERVER_MSG_SIZE, 0);
            }

            while(wait(NULL) > 0);

            msg.mtype = 1;
            msgsnd(mq_descriptor, &msg, db_ipc::MANAGER_SERVER_MSG_SIZE, 0);

            break;
        }
        else if (cmd == "run")
        {
            msg.cmd = db_ipc::command::ADD_STORAGE_SERVER;
            msg.mtype = 4;

            msgsnd(mq_descriptor, &msg, db_ipc::MANAGER_SERVER_MSG_SIZE, 0);
        }
        else if (cmd == "terminate")
        {
            msg.cmd = db_ipc::command::TERMINATE_STORAGE_SERVER;
            msg.mtype = 4;

            msgsnd(mq_descriptor, &msg, db_ipc::MANAGER_SERVER_MSG_SIZE, 0);
        }
    }
}

void run_assistant_reader()
{
    db_ipc::strg_msg_t msg;

    std::cout << "Assistant started" << std::endl;

    while (true)
    {
        ssize_t rcv_cnt = msgrcv(mq_descriptor, &msg, db_ipc::MANAGER_SERVER_MSG_SIZE, db_ipc::MANAGER_SERVER_CLIENT_PING_PRIOR, MSG_NOERROR);

        if (msg.cmd == db_ipc::command::TERMINATE_ASSISTANT_THREAD)
        {
            break;
        }


        std::cout << "Assistant send" << std::endl;

        msg.status = db_ipc::command_status::SERVER_IS_BUSY;
        msg.mtype = msg.pid;
        msgsnd(mq_descriptor, &msg, db_ipc::MANAGER_SERVER_MSG_SIZE, MSG_NOERROR);
    }


    std::cout << "Assistant end" << std::endl;
}

#pragma region ipc utility

int msgrcvt(
        int max_counter,
        int descriptor,
        db_ipc::strg_msg_t &msg,
        size_t msg_size,
        long mtypes,
        int flags = 0)
{
    ssize_t rcv = -1;
    int counter = 0;
    do
    {
        rcv = msgrcv(descriptor, &msg, msg_size, mtypes, flags | IPC_NOWAIT);
        usleep(counter < 5 ? 250 : counter * 500);
    } while (++counter < max_counter && rcv == -1);

    return rcv;
}

int transfer_left(
        std::vector<pid_t> const &strg_servers,
        std::map<db_path, std::vector<std::string>> &separators,
        db_path const &path,
        int id)
{
    std::string new_separator;
    db_ipc::strg_msg_t msg;
    msg.pid = getpid();
    strcpy(msg.pool_name, path.pool.c_str());
    strcpy(msg.schema_name, path.schema.c_str());
    strcpy(msg.collection_name, path.collection.c_str());

    // GET CURRENT SERVER MIN
    msg.mtype = strg_servers.at(id);
    msg.status = db_ipc::command_status::OK;
    msg.cmd = db_ipc::command::OBTAIN_MIN;

    msgsnd(mq_descriptor, &msg, db_ipc::MANAGER_SERVER_MSG_SIZE, MSG_NOERROR);
    ssize_t rcv = msgrcvt(10, mq_descriptor, msg, db_ipc::MANAGER_SERVER_MSG_SIZE,
                          db_ipc::MANAGER_SERVER_STORAGE_ANSWER_PRIOR);

    if (rcv == -1 || msg.status != db_ipc::command_status::OK) return -1;

    // GET NEXT TO CURRENT SERVER MIN
    msg.mtype = strg_servers.at(id);
    msg.status = db_ipc::command_status::OK;
    msg.cmd = db_ipc::command::OBTAIN_NEXT;

    msgsnd(mq_descriptor, &msg, db_ipc::MANAGER_SERVER_MSG_SIZE, MSG_NOERROR);
    rcv = msgrcvt(10, mq_descriptor, msg, db_ipc::MANAGER_SERVER_MSG_SIZE,
                  db_ipc::MANAGER_SERVER_STORAGE_ANSWER_PRIOR);

    if (msg.status == db_ipc::command_status::ATTTEMT_TO_OBTAIN_NONEXISTENT_KEY)
    {
        msg.status == db_ipc::command_status::OK;
    }

    if (rcv == -1 || msg.status != db_ipc::command_status::OK) return -1;

    new_separator = msg.login;

    // SEND MIN TO THE LEFT SERVER
    msg.mtype = strg_servers.at(id - 1);
    msg.cmd = db_ipc::command::ADD;

    msgsnd(mq_descriptor, &msg, db_ipc::MANAGER_SERVER_MSG_SIZE, MSG_NOERROR);
    rcv = msgrcvt(10, mq_descriptor, msg, db_ipc::MANAGER_SERVER_MSG_SIZE,
                  db_ipc::MANAGER_SERVER_STORAGE_ANSWER_PRIOR);

    if (rcv == -1 || msg.status != db_ipc::command_status::OK) return -1;

    // DISPOSE MIN FROM CUR SERVER
    msg.mtype = strg_servers.at(id);
    msg.cmd = db_ipc::command::DISPOSE;

    msgsnd(mq_descriptor, &msg, db_ipc::MANAGER_SERVER_MSG_SIZE, MSG_NOERROR);
    rcv = msgrcvt(25, mq_descriptor, msg, db_ipc::MANAGER_SERVER_MSG_SIZE,
                  db_ipc::MANAGER_SERVER_STORAGE_ANSWER_PRIOR);

    if (rcv == -1 || msg.status != db_ipc::command_status::OK)
    {
        std::cout << "WORST CASE" << std::endl;
    }

    separators[path][id - 1] = new_separator;

    return 0;
}

int transfer_right(
        std::vector<pid_t> const &strg_servers,
        std::map<db_path, std::vector<std::string>> &separators,
        db_path const &path,
        int id)
{
    std::string new_separator;
    db_ipc::strg_msg_t msg;
    msg.pid = getpid();
    strcpy(msg.pool_name, path.pool.c_str());
    strcpy(msg.schema_name, path.schema.c_str());
    strcpy(msg.collection_name, path.collection.c_str());

    // GET CURRENT SERVER MAX
    msg.mtype = strg_servers.at(id);
    msg.status = db_ipc::command_status::OK;
    msg.cmd = db_ipc::command::OBTAIN_MAX;

    msgsnd(mq_descriptor, &msg, db_ipc::MANAGER_SERVER_MSG_SIZE, MSG_NOERROR);
    ssize_t rcv = msgrcvt(10, mq_descriptor, msg, db_ipc::MANAGER_SERVER_MSG_SIZE,
                          db_ipc::MANAGER_SERVER_STORAGE_ANSWER_PRIOR);

    if (rcv == -1 || msg.status != db_ipc::command_status::OK) return -1;

    new_separator = msg.login;

    // SEND MAX TO THE RIGHT SERVER
    msg.mtype = strg_servers.at(id + 1);
    msg.cmd = db_ipc::command::ADD;

    msgsnd(mq_descriptor, &msg, db_ipc::MANAGER_SERVER_MSG_SIZE, MSG_NOERROR);
    rcv = msgrcvt(10, mq_descriptor, msg, db_ipc::MANAGER_SERVER_MSG_SIZE,
                  db_ipc::MANAGER_SERVER_STORAGE_ANSWER_PRIOR);

    if (rcv == -1 || msg.status != db_ipc::command_status::OK) return -1;

    // DISPOSE MAX FROM CUR SERVER
    msg.mtype = strg_servers.at(id);
    msg.cmd = db_ipc::command::DISPOSE;

    msgsnd(mq_descriptor, &msg, db_ipc::MANAGER_SERVER_MSG_SIZE, MSG_NOERROR);
    rcv = msgrcvt(10, mq_descriptor, msg, db_ipc::MANAGER_SERVER_MSG_SIZE,
                  db_ipc::MANAGER_SERVER_STORAGE_ANSWER_PRIOR);

    if (rcv == -1 || msg.status != db_ipc::command_status::OK)
    {
        std::cout << "WORST CASE" << std::endl;
    }

    separators[path][id] = new_separator;

    return 0;
}

int transfer_left_n(
        std::vector<pid_t> const &strg_servers,
        std::map<db_path, std::vector<std::string>> &separators,
        db_path const &path,
        int id,
        int n)
{
    for (size_t i = 0; i < n; ++i)
    {
        int res = transfer_left(strg_servers, separators, path, id);
        if (res) return res;
    }

    return 0;
}

int transfer_right_n(
        std::vector<pid_t> const &strg_servers,
        std::map<db_path, std::vector<std::string>> &separators,
        db_path const &path,
        int id,
        int n)
{
    for (size_t i = 0; i < n; ++i)
    {
        int res = transfer_right(strg_servers, separators, path, id);
        if (res) return res;
    }

    return 0;
}

#pragma endregion ipc utility

void handle_add_server_command(
        bool is_filesystem,
        std::vector<pid_t> &strg_servers,
        std::map<db_path, std::vector<std::string>> &separators,
        db_ipc::strg_msg_t &msg)
{
    size_t id = strg_servers.size() + 1;

    pid_t pid = run_storage_server(id);

    if (pid == -1)
    {
        std::cout << "Failed to add storage server" << std::endl;
        return;
    }

    msg.mtype = pid;
    msg.cmd = is_filesystem ? db_ipc::command::SET_FILE_SYSTEM_MODE : db_ipc::command::SET_IN_MEMORY_CACHE_MODE;
    msg.extra_value = id;
    msg.pid = getpid();

    msgsnd(mq_descriptor, &msg, db_ipc::MANAGER_SERVER_MSG_SIZE, MSG_NOERROR);
    ssize_t rcv_cnt = msgrcvt(10, mq_descriptor, msg, db_ipc::MANAGER_SERVER_MSG_SIZE,
                              db_ipc::MANAGER_SERVER_STORAGE_ADDITION_PRIOR);

    if (rcv_cnt == -1 || msg.status == db_ipc::command_status::FAILED_TO_SETUP_STORAGE_SERVER)
    {
        kill(pid, SIGKILL);
        std::cout << "Failed to add storage server" << std::endl;
        return;
    }

    strg_servers.push_back(pid);

    if (id > 1)
    {
        for (auto &pair : separators)
        {
            pair.second.push_back(std::string(db_ipc::MSG_KEY_SIZE, 127));
        }

        redistribute_keys(strg_servers, separators);
    }
}

void handle_terminate_server_command(
        std::vector<pid_t> &strg_servers,
        std::map<db_path, std::vector<std::string>> &separators,
        db_ipc::strg_msg_t &msg)
{
    if (strg_servers.size() == 1)
    {
        throw std::runtime_error("Cannot terminate single server");
    }

    size_t id = strg_servers.size() - 1;
    msg.cmd = db_ipc::command::GET_RECORDS_CNT;
    msg.pid = getpid();

    for (auto [path, sep_vector] : separators)
    {
        msg.status = db_ipc::command_status::OK;
        strcpy(msg.pool_name, path.pool.c_str());
        strcpy(msg.schema_name, path.schema.c_str());
        strcpy(msg.collection_name, path.collection.c_str());

        msg.mtype = strg_servers.at(id);
        msgsnd(mq_descriptor, &msg, db_ipc::MANAGER_SERVER_MSG_SIZE, MSG_NOERROR);

        ssize_t rcv = msgrcvt(25, mq_descriptor, msg, db_ipc::MANAGER_SERVER_MSG_SIZE,
                              db_ipc::MANAGER_SERVER_STORAGE_GETTING_RECORDS_CNT_PRIOR);

        if (rcv == -1)
        {
            throw std::runtime_error("Failed to terminate server");
        }

        if (transfer_left_n(strg_servers, separators, path, id, msg.extra_value) == -1)
        {
            throw std::runtime_error("Failed to terminate server");
        }
    }

    msg.mtype = strg_servers[id];
    msg.cmd = db_ipc::command::TERMINATE;
    msgsnd(mq_descriptor, &msg, db_ipc::MANAGER_SERVER_MSG_SIZE, 0);

    strg_servers.erase(strg_servers.begin() + id);

    redistribute_keys(strg_servers, separators);
}

pid_t run_storage_server(
        size_t strg_server_id)
{
    pid_t pid = fork();

    if (pid == 0)
    {
        int code = execl("../db_storage/server/os_cw_dbms_db_strg_srvr", std::to_string(strg_server_id).c_str(), NULL);

        if (code == -1)
        {
            exit(1);
        }
    }

    if (pid == -1)
    {
        std::cout << "fork error" << std::endl;
        // todo throw;
        return -1;
    }

    usleep(200);

    if (waitpid(pid, NULL, WNOHANG) == 0)
    {
        return pid;
    }

    return -1;
}


size_t get_id_for_key(
        std::map<db_path, std::vector<std::string>> const &separators,
        db_path path,
        std::string const &key)
{
    // TODO binary search

    auto comparer = tkey_comparer();

    if (separators.empty())
    {
        return 0;
    }

    auto const &vector = separators.at(path);
    size_t id = 0;

    for (size_t i = 0; i < vector.size() && comparer(key, vector[i]) >= 0; ++i)
    {
        ++id;
    }

    return id;
}

int redistribute_path_keys(
        std::vector<pid_t> const &strg_servers,
        std::map<db_path, std::vector<std::string>> &separators,
        db_path const &path)
{
    size_t sz = strg_servers.size();
    db_ipc::strg_msg_t msg;

    msg.status = db_ipc::command_status::OK;
    msg.cmd = db_ipc::command::GET_RECORDS_CNT;
    msg.pid = getpid();

    strcpy(msg.pool_name, path.pool.c_str());
    strcpy(msg.schema_name, path.schema.c_str());
    strcpy(msg.collection_name, path.collection.c_str());

    std::vector<size_t> cnts(sz);
    size_t sum = 0;

    for (size_t i = 0; i < sz; ++i)
    {
        msg.mtype = strg_servers.at(i);
        msgsnd(mq_descriptor, &msg, db_ipc::MANAGER_SERVER_MSG_SIZE, MSG_NOERROR);

        ssize_t rcv = msgrcvt(20, mq_descriptor, msg, db_ipc::MANAGER_SERVER_MSG_SIZE,
                              db_ipc::MANAGER_SERVER_STORAGE_GETTING_RECORDS_CNT_PRIOR);

        if (rcv == -1)
        {
            return -1;
        }

        cnts[i] = msg.extra_value;
        sum += cnts[i];
    }

    if (sum < sz)
    {
        return 0;
    }
    // if (sum < 2 * sz)
    // {
    //     return 0;
    // }

    size_t target_cnt = sum / sz;
    size_t additional = sum % sz;
    bool need_redistribution = false;

    for (size_t i = 0; i < sz; ++i)
    {
        size_t cur_target = target_cnt + (i < additional ? 1 : 0);
        if (cnts[i] == 0 || cur_target / cnts[i] > 2 || cnts[i] / cur_target > 2)
        {
            need_redistribution = true;
            break;
        }
    }

    if (!need_redistribution)
    {
        return 0;
    }

    std::vector<long long> differ(sz);
    std::vector<long long> differ_prefix(sz, 0);

    for (size_t i = 0; i < sz; ++i)
    {
        differ[i] = cnts[i] - (target_cnt + (i < additional ? 1 : 0));
        differ_prefix[i] = (i > 0 ? differ_prefix[i-1] : 0) + differ[i];
    }

    for (size_t i = 0; i < sz; ++i)
    {
        if (differ_prefix[i] >= 0)
        {
            if (i > 0 && differ_prefix[i - 1] < 0)
            {
                for (size_t j = i; j > 0; --j)
                {
                    int res = transfer_left_n(strg_servers, separators, path, j, -differ_prefix[j - 1]);
                    if (res) return res;
                }
                differ[i] += differ_prefix[i - 1];
            }
            if (i + 1 < sz && differ[i] > 0)
            {
                int res = transfer_right_n(strg_servers, separators, path, i, differ[i]);
                if (res) return res;
                differ[i + 1] += differ[i];
            }

            differ_prefix[i] = 0;
        }
    }

    return 0;
}

int redistribute_keys(
        std::vector<pid_t> const &strg_servers,
        std::map<db_path, std::vector<std::string>> &separators)
{
    int main_res = 0;

    // std::thread assistant_thread(run_assistant_reader);

    for (auto &pair : separators)
    {
        int res = redistribute_path_keys(strg_servers, separators, pair.first);
        if (res)
        {
            std::cout << "Keys distribution aborted due to some error" << std::endl;
        }
        main_res |= res;
    }

    db_ipc::strg_msg_t msg;
    msg.mtype = db_ipc::MANAGER_SERVER_CLIENT_PING_PRIOR;
    msg.cmd = db_ipc::command::TERMINATE_ASSISTANT_THREAD;

    msgsnd(mq_descriptor, &msg, db_ipc::MANAGER_SERVER_MSG_SIZE, MSG_NOERROR);
    // assistant_thread.join();

    return main_res;
}

void handle_add_struct_command(
        std::vector<pid_t> const &strg_servers,
        std::map<db_path, std::vector<std::string>> &separators,
        db_ipc::strg_msg_t &msg)
{
    if (strg_servers.empty())
    {
        msg.mtype = msg.pid;
        msg.status = db_ipc::command_status::FAILED_TO_ADD_STRUCT;
        msgsnd(mq_descriptor, &msg, db_ipc::MANAGER_SERVER_MSG_SIZE, MSG_NOERROR);
    }
    else
    {
        for (size_t i = 0; i < strg_servers.size(); ++i)
        {
            msg.mtype = strg_servers.at(i);
            msgsnd(mq_descriptor, &msg, db_ipc::MANAGER_SERVER_MSG_SIZE, MSG_NOERROR);

            ssize_t rcv = msgrcvt(20, mq_descriptor, msg, db_ipc::MANAGER_SERVER_MSG_SIZE,
                                  db_ipc::MANAGER_SERVER_STRUCT_ADDITION_PRIOR);

            if (rcv == -1 || msg.status != db_ipc::command_status::OK)
            {
                db_ipc::command_status old_status = msg.status != db_ipc::command_status::OK ? msg.status : db_ipc::command_status::FAILED_TO_ADD_STRUCT;
                db_ipc::command old_cmd = msg.cmd;

                switch (msg.cmd)
                {
                    case db_ipc::command::ADD_POOL:       msg.cmd = db_ipc::command::DISPOSE_POOL; break;
                    case db_ipc::command::ADD_SCHEMA:     msg.cmd = db_ipc::command::DISPOSE_SCHEMA; break;
                    case db_ipc::command::ADD_COLLECTION: msg.cmd = db_ipc::command::DISPOSE_COLLECTION; break;
                }

                for (size_t j = i; j > 0; ++i)
                {
                    msg.status = db_ipc::command_status::OK;
                    msg.mtype = strg_servers.at(j - 1);

                    msgsnd(mq_descriptor, &msg, db_ipc::MANAGER_SERVER_MSG_SIZE, MSG_NOERROR);

                    rcv = msgrcvt(20, mq_descriptor, msg, db_ipc::MANAGER_SERVER_MSG_SIZE,
                                  db_ipc::MANAGER_SERVER_STRUCT_DISPOSAL_PRIOR);

                    if (rcv == -1 || msg.status != db_ipc::command_status::OK)
                    {
                        std::cout << "pu pu pu" << std::endl;
                        // TODO;
                    }
                }

                msg.mtype = msg.pid;
                msg.cmd = old_cmd;
                msg.status = old_status;
                msgsnd(mq_descriptor, &msg, db_ipc::MANAGER_SERVER_MSG_SIZE, MSG_NOERROR);
                return;
            }
        }

        if (msg.cmd == db_ipc::command::ADD_COLLECTION)
        {
            separators[db_path(msg.pool_name, msg.schema_name, msg.collection_name)] =
                    std::vector<std::string>(strg_servers.size() - 1, std::string(db_ipc::MSG_KEY_SIZE, 127));
        }

        msg.mtype = msg.pid;
        msg.status = db_ipc::command_status::OK;
        msgsnd(mq_descriptor, &msg, db_ipc::MANAGER_SERVER_MSG_SIZE, MSG_NOERROR);
    }
}

void handle_dispose_struct_command(
        std::vector<pid_t> const &strg_servers,
        std::map<db_path, std::vector<std::string>> &separators,
        db_ipc::strg_msg_t &msg)
{
    if (strg_servers.empty())
    {
        msg.mtype = msg.pid;
        msg.status = db_ipc::command_status::FAILED_TO_DISPOSE_KEY;
        msgsnd(mq_descriptor, &msg, db_ipc::MANAGER_SERVER_MSG_SIZE, MSG_NOERROR);
    }
    else
    {
        for (size_t i = 0; i < strg_servers.size(); ++i)
        {
            msg.mtype = strg_servers.at(i);
            msgsnd(mq_descriptor, &msg, db_ipc::MANAGER_SERVER_MSG_SIZE, MSG_NOERROR);

            ssize_t rcv = msgrcvt(20, mq_descriptor, msg, db_ipc::MANAGER_SERVER_MSG_SIZE,
                                  db_ipc::MANAGER_SERVER_STRUCT_DISPOSAL_PRIOR);

            if (rcv == -1 || msg.status != db_ipc::command_status::OK)
            {
                std::cout << "pu pu pu" << std::endl;
                // TODO;
            }
        }

        if (msg.cmd == db_ipc::command::DISPOSE_COLLECTION)
        {
            separators.erase(db_path(msg.pool_name, msg.schema_name, msg.collection_name));
        }

        msg.mtype = msg.pid;
        msg.status = db_ipc::command_status::OK;
        msgsnd(mq_descriptor, &msg, db_ipc::MANAGER_SERVER_MSG_SIZE, MSG_NOERROR);
        return;
    }
}

void handle_data_command(
        std::vector<pid_t> const &strg_servers,
        std::map<db_path, std::vector<std::string>> const &separators,
        db_ipc::strg_msg_t &msg)
{
    if (msg.status == db_ipc::command_status::CLIENT)
    {
        if (strg_servers.empty())
        {
            msg.mtype = msg.pid;
            msg.status = db_ipc::command_status::FAILED_TO_PERFORM_DATA_COMMAND;
            msgsnd(mq_descriptor, &msg, db_ipc::MANAGER_SERVER_MSG_SIZE, MSG_NOERROR);
        }
        else
        {
            size_t id = get_id_for_key(separators, db_path(msg.pool_name, msg.schema_name, msg.collection_name), msg.login);
            msg.mtype = strg_servers.at(id);
            msgsnd(mq_descriptor, &msg, db_ipc::MANAGER_SERVER_MSG_SIZE, MSG_NOERROR);
        }
    }
    else
    {
        msg.mtype = msg.pid;
        msgsnd(mq_descriptor, &msg, db_ipc::MANAGER_SERVER_MSG_SIZE, MSG_NOERROR);
    }
}

void handle_obtain_between_command(
        std::vector<pid_t> const &strg_servers,
        std::map<db_path, std::vector<std::string>> const &separators,
        db_ipc::strg_msg_t &msg)
{
    if (msg.status == db_ipc::command_status::CLIENT)
    {
        if (strg_servers.empty())
        {
            msg.mtype = msg.pid;
            msg.status = db_ipc::command_status::FAILED_TO_PERFORM_DATA_COMMAND;
            msgsnd(mq_descriptor, &msg, db_ipc::MANAGER_SERVER_MSG_SIZE, MSG_NOERROR);
        }
        else
        {
            size_t left_id = get_id_for_key(separators, db_path(msg.pool_name, msg.schema_name, msg.collection_name), msg.login);
            size_t right_id = get_id_for_key(separators, db_path(msg.pool_name, msg.schema_name, msg.collection_name), msg.right_boundary_login);

            msg.extra_value = right_id - left_id + 1;

            for (size_t id = left_id; id <= right_id; ++id)
            {
                msg.mtype = strg_servers.at(id);
                msgsnd(mq_descriptor, &msg, db_ipc::MANAGER_SERVER_MSG_SIZE, MSG_NOERROR);
            }
        }
    }
    else
    {
        msg.mtype = msg.pid;
        msgsnd(mq_descriptor, &msg, db_ipc::MANAGER_SERVER_MSG_SIZE, MSG_NOERROR);
    }
}