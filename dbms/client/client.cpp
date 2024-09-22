#include <iostream>
#include <sstream>
#include <fstream>
#include <string.h>
#include <unistd.h>
#include <sys/msg.h>

#include <ipc_data.h>

struct tvalue
{
	int64_t hashed_passwod;
	std::string name;
};

int msgrcvt(
    int max_counter,
    int descriptor,
    db_ipc::strg_msg_t &msg,
    size_t msg_size,
    long mtypes,
    int flags = 0);

void run_session(
	int mq_descriptor,
    std::istream &stream,
	bool is_terminal,
	size_t &request_counter);

int main()
{	
	pid_t pid = getpid();
	
	while (getpid() <= db_ipc::MANAGER_SERVER_MAX_COMMAND_PRIOR)
	{
		switch (pid = fork())
		{
			case -1:
				std::cout << "An error occurred while starting client process" << std::endl;
				return 1;
			case 0:
				break;
			default:
				waitpid(pid, NULL, 0);
				return 0;
		}
	}
	
	int mq_descriptor = msgget(db_ipc::MANAGER_SERVER_MQ_KEY, 0666);
	size_t request_counter = 1;
	
	if (mq_descriptor == -1)
	{
		std::cout << "Cannot connect to the server" << std::endl;
		return 2;
	}
	
	int rcv = -1;
	do
	{
		db_ipc::strg_msg_t msg;
		rcv = msgrcv(mq_descriptor, &msg, db_ipc::MANAGER_SERVER_MSG_SIZE, getpid(), IPC_NOWAIT);
	} while (rcv != -1);
	
	try
	{
		run_session(mq_descriptor, std::cin, true, request_counter);
	}
	catch (std::runtime_error const &ex)
	{
		std::cout << ex.what() << std::endl;
		return 3;
	}
}



db_ipc::allocator_variant read_allocator(
	std::istringstream &stream)
{
	std::string allocator;
	
	if (!(stream >> allocator))
	{
		throw std::runtime_error("Expected allocator type");
	}
	
    if (allocator == "global_heap")
    {
        return db_ipc::allocator_variant::GLOBAL_HEAP;
    }
    else if (allocator == "boundary_tags")
    {
        return db_ipc::allocator_variant::BOUNDARY_TAGS;
    }
    else if (allocator == "buddies_system")
    {
        return db_ipc::allocator_variant::BUDDY_SYSTEM;
    }
    else if (allocator == "sorted_list")
    {
        return db_ipc::allocator_variant::SORTED_LIST;
    }
    else if (allocator == "red_black_tree")
    {
        return db_ipc::allocator_variant::RED_BLACK_TREE;
    }
	
	throw std::runtime_error("Invalid allocator type");
}

db_ipc::allocator_fit_mode read_allocator_fit_mode(
	std::istringstream &stream)
{
	std::string fit_mode;
	
	if (!(stream >> fit_mode))
	{
		throw std::runtime_error("Expected allocator fit mode");
	}
	
    if (fit_mode == "first")
    {
        return db_ipc::allocator_fit_mode::FIRST_FIT;
    }
    else if (fit_mode == "best")
    {
        return db_ipc::allocator_fit_mode::THE_BEST_FIT;
    }
    else if (fit_mode == "worst")
    {
        return db_ipc::allocator_fit_mode::THE_WORST_FIT;
    }
	
	throw std::runtime_error("Invalid allocator fit mode");
}

std::string read_key(
	std::istringstream &args)
{
	std::string key;
	
	if (!(args >> key))
	{
		throw std::runtime_error("Expected key");
	}
	if (key.size() == 0)
	{
		throw std::runtime_error("Invalid key");
	}
	
    return key;
}

tvalue read_value(
	std::istringstream &args)
{
    tvalue value;
	
    if (!(args >> value.hashed_passwod && args >> value.name))
    {
		throw std::runtime_error("Expected value");
    }

    return value;
}

std::string read_struct_name(
	std::istringstream &args)
{
    std::string name;
	
    if (!(args >> name))
    {
		throw std::runtime_error("Expected struct name");
    }
	if (name.size() > db_ipc::MSG_STRUCTS_NAME_SIZE)
    {
		throw std::runtime_error("Invalid struct name");
    }

    return name;
}

size_t read_parameter_t_for_b_trees(
	std::istringstream &args)
{
    size_t t;
	
    if (!(args >> t))
    {
		throw std::runtime_error("Expected BTree parameter");
    }
    if (t < 2)
    {
		throw std::runtime_error("Invalid BTree parameter");
    }

    return t;
}

void validate_eof(
	std::istringstream &args)
{
	if (!args.eof())
	{
		throw std::runtime_error("Excess arguments");
	}
}

void handle_add_command(
	int mq_descriptor,
	std::istringstream &args,
	db_ipc::strg_msg_t &msg)
{
	std::string pool_name = read_struct_name(args);
	std::string schema_name = read_struct_name(args);
	std::string collection_name = read_struct_name(args);
	std::string key = read_key(args);
	tvalue value = read_value(args);
	validate_eof(args);
	
	msg.mtype = db_ipc::MANAGER_SERVER_CLIENT_PRIOR;
	msg.pid = getpid();
	msg.cmd = db_ipc::command::ADD;
	msg.status = db_ipc::command_status::CLIENT;
	
	strcpy(msg.pool_name, pool_name.c_str());
	strcpy(msg.schema_name, schema_name.c_str());
	strcpy(msg.collection_name, collection_name.c_str());
	
	strcpy(msg.login, key.c_str());
	msg.hashed_password = value.hashed_passwod;
	strcpy(msg.name, value.name.c_str());
	
	int snd = msgsnd(mq_descriptor, &msg, db_ipc::MANAGER_SERVER_MSG_SIZE, 0);
	if (snd == -1)
	{
		throw std::runtime_error("Failed to send command to the server");
	}
}

void handle_update_command(
	int mq_descriptor,
	std::istringstream &args,
	db_ipc::strg_msg_t &msg)
{
	std::string pool_name = read_struct_name(args);
	std::string schema_name = read_struct_name(args);
	std::string collection_name = read_struct_name(args);
	std::string key = read_key(args);
	tvalue value = read_value(args);
	validate_eof(args);
	
	msg.mtype = 10;
	msg.pid = getpid();
	msg.cmd = db_ipc::command::UPDATE;
	msg.status = db_ipc::command_status::CLIENT;
	
	strcpy(msg.pool_name, pool_name.c_str());
	strcpy(msg.schema_name, schema_name.c_str());
	strcpy(msg.collection_name, collection_name.c_str());
	
	strcpy(msg.login, key.c_str());
	msg.hashed_password = value.hashed_passwod;
	strcpy(msg.name, value.name.c_str());
	
	int snd = msgsnd(mq_descriptor, &msg, db_ipc::MANAGER_SERVER_MSG_SIZE, 0);
	if (snd == -1)
	{
		throw std::runtime_error("Failed to send command to the server");
	}
}

void handle_dispose_command(
	int mq_descriptor,
	std::istringstream &args,
	db_ipc::strg_msg_t &msg)
{
	std::string pool_name = read_struct_name(args);
	std::string schema_name = read_struct_name(args);
	std::string collection_name = read_struct_name(args);
	std::string key = read_key(args);
	validate_eof(args);
	
	msg.mtype = 10;
	msg.pid = getpid();
	msg.cmd = db_ipc::command::DISPOSE;
	msg.status = db_ipc::command_status::CLIENT;
	
	strcpy(msg.pool_name, pool_name.c_str());
	strcpy(msg.schema_name, schema_name.c_str());
	strcpy(msg.collection_name, collection_name.c_str());
	
	strcpy(msg.login, key.c_str());
	
	int snd = msgsnd(mq_descriptor, &msg, db_ipc::MANAGER_SERVER_MSG_SIZE, 0);
	if (snd == -1)
	{
		throw std::runtime_error("Failed to send command to the server");
	}
}

void handle_obtain_command(
	int mq_descriptor,
	std::istringstream &args,
	db_ipc::strg_msg_t &msg)
{
	std::string pool_name = read_struct_name(args);
	std::string schema_name = read_struct_name(args);
	std::string collection_name = read_struct_name(args);
	std::string key1 = read_key(args);
	std::string key2 = "";
	if (!args.eof()) key2 = read_key(args);
	validate_eof(args);
	
	msg.mtype = 10;
	msg.pid = getpid();
	msg.cmd = db_ipc::command::OBTAIN;
	msg.status = db_ipc::command_status::CLIENT;
	
	if (key2[0])
	{
		msg.cmd = db_ipc::command::OBTAIN_BETWEEN;
	}
	
	strcpy(msg.pool_name, pool_name.c_str());
	strcpy(msg.schema_name, schema_name.c_str());
	strcpy(msg.collection_name, collection_name.c_str());
	
	strcpy(msg.login, key1.c_str());
	strcpy(msg.right_boundary_login, key2.c_str());
	
	int snd = msgsnd(mq_descriptor, &msg, db_ipc::MANAGER_SERVER_MSG_SIZE, 0);
	if (snd == -1)
	{
		throw std::runtime_error("Failed to send command to the server");
	}
}

void handle_add_pool_command(
	int mq_descriptor,
	std::istringstream &args,
	db_ipc::strg_msg_t &msg)
{
	std::string pool_name = read_struct_name(args);
	size_t t = read_parameter_t_for_b_trees(args);
	validate_eof(args);
	
	msg.mtype = 10;
	msg.pid = getpid();
	msg.cmd = db_ipc::command::ADD_POOL;
	msg.status = db_ipc::command_status::CLIENT;
	
	strcpy(msg.pool_name, pool_name.c_str());
	
	msg.tree_variant = db_ipc::search_tree_variant::B;
	msg.t_for_b_trees = t;
	
	int snd = msgsnd(mq_descriptor, &msg, db_ipc::MANAGER_SERVER_MSG_SIZE, 0);
	if (snd == -1)
	{
		throw std::runtime_error("Failed to send command to the server");
	}
}

void handle_add_schema_command(
	int mq_descriptor,
	std::istringstream &args,
	db_ipc::strg_msg_t &msg)
{
	std::string pool_name = read_struct_name(args);
	std::string schema_name = read_struct_name(args);
	size_t t = read_parameter_t_for_b_trees(args);
	validate_eof(args);
	
	msg.mtype = 10;
	msg.pid = getpid();
	msg.cmd = db_ipc::command::ADD_SCHEMA;
	msg.status = db_ipc::command_status::CLIENT;
	
	strcpy(msg.pool_name, pool_name.c_str());
	strcpy(msg.schema_name, schema_name.c_str());
	
	msg.tree_variant = db_ipc::search_tree_variant::B;
	msg.t_for_b_trees = t;
	
	int snd = msgsnd(mq_descriptor, &msg, db_ipc::MANAGER_SERVER_MSG_SIZE, 0);
	if (snd == -1)
	{
		throw std::runtime_error("Failed to send command to the server");
	}
}

void handle_add_collection_command(
	int mq_descriptor,
	std::istringstream &args,
	db_ipc::strg_msg_t &msg)
{
	std::string pool_name = read_struct_name(args);
	std::string schema_name = read_struct_name(args);
	std::string collection_name = read_struct_name(args);
	size_t t = read_parameter_t_for_b_trees(args);
	db_ipc::allocator_variant alloc_variant = read_allocator(args);
	db_ipc::allocator_fit_mode alloc_fit_mode = read_allocator_fit_mode(args);
	validate_eof(args);
	
	msg.mtype = 10;
	msg.pid = getpid();
	msg.cmd = db_ipc::command::ADD_COLLECTION;
	msg.status = db_ipc::command_status::CLIENT;
	
	strcpy(msg.pool_name, pool_name.c_str());
	strcpy(msg.schema_name, schema_name.c_str());
	strcpy(msg.collection_name, collection_name.c_str());
	
	msg.tree_variant = db_ipc::search_tree_variant::B;
	msg.t_for_b_trees = t;
	msg.alloc_variant = alloc_variant;
	msg.alloc_fit_mode = alloc_fit_mode;
	
	int snd = msgsnd(mq_descriptor, &msg, db_ipc::MANAGER_SERVER_MSG_SIZE, 0);
	if (snd == -1)
	{
		throw std::runtime_error("Failed to send command to the server");
	}
}

void handle_dispose_pool_command(
	int mq_descriptor,
	std::istringstream &args,
	db_ipc::strg_msg_t &msg)
{
	std::string pool_name = read_struct_name(args);
	validate_eof(args);
	
	msg.mtype = 10;
	msg.pid = getpid();
	msg.cmd = db_ipc::command::DISPOSE_POOL;
	msg.status = db_ipc::command_status::CLIENT;
	
	strcpy(msg.pool_name, pool_name.c_str());
	
	int snd = msgsnd(mq_descriptor, &msg, db_ipc::MANAGER_SERVER_MSG_SIZE, 0);
	if (snd == -1)
	{
		throw std::runtime_error("Failed to send command to the server");
	}
}

void handle_dispose_schema_command(
	int mq_descriptor,
	std::istringstream &args,
	db_ipc::strg_msg_t &msg)
{
	std::string pool_name = read_struct_name(args);
	std::string schema_name = read_struct_name(args);
	validate_eof(args);
	
	msg.mtype = 10;
	msg.pid = getpid();
	msg.cmd = db_ipc::command::DISPOSE_SCHEMA;
	msg.status = db_ipc::command_status::CLIENT;
	
	strcpy(msg.pool_name, pool_name.c_str());
	strcpy(msg.schema_name, schema_name.c_str());
	
	int snd = msgsnd(mq_descriptor, &msg, db_ipc::MANAGER_SERVER_MSG_SIZE, 0);
	if (snd == -1)
	{
		throw std::runtime_error("Failed to send command to the server");
	}
}

void handle_dispose_collection_command(
	int mq_descriptor,
	std::istringstream &args,
	db_ipc::strg_msg_t &msg)
{
	std::string pool_name = read_struct_name(args);
	std::string schema_name = read_struct_name(args);
	std::string collection_name = read_struct_name(args);
	validate_eof(args);
	
	msg.mtype = 10;
	msg.pid = getpid();
	msg.cmd = db_ipc::command::DISPOSE_COLLECTION;
	msg.status = db_ipc::command_status::CLIENT;
	
	strcpy(msg.pool_name, pool_name.c_str());
	strcpy(msg.schema_name, schema_name.c_str());
	strcpy(msg.collection_name, collection_name.c_str());
	
	int snd = msgsnd(mq_descriptor, &msg, db_ipc::MANAGER_SERVER_MSG_SIZE, 0);
	if (snd == -1)
	{
		throw std::runtime_error("Failed to send command to the server");
	}
}

void handle_execute_file_command(
	int mq_descriptor,
	std::istringstream &args,
	size_t &request_counter)
{
	std::string path;
	
	args >> path;
	validate_eof(args);
	
	std::ifstream fstream(path);
	
	if (!fstream.is_open())
	{
		throw std::runtime_error("Cannot open the command file");
	}
	
	run_session(mq_descriptor, fstream, false, request_counter);
}

void handle_reconnect_command(
	int &mq_descriptor,
	std::istringstream &args)
{
	validate_eof(args);
	
	mq_descriptor = msgget(db_ipc::MANAGER_SERVER_MQ_KEY, 0666);
	
	if (mq_descriptor == -1)
	{
		throw std::runtime_error("Cannot reconnect to the server");
	}
	
	std::cout << "Reconnected" << std::endl << std::endl;
}

void handle_server_answer(
	int mq_descriptor,
	db_ipc::strg_msg_t &msg,
	size_t request_counter)
{
	char login[db_ipc::MSG_KEY_SIZE];
	strcpy(login, msg.login);
	
	int rcv = -1;
	msg.req_id = 0;
	
	while (request_counter != msg.req_id)
	{
		rcv = msgrcvt(60, mq_descriptor, msg, db_ipc::MANAGER_SERVER_MSG_SIZE, getpid());
		
		if (msg.cmd == db_ipc::command::PING)
		{
			rcv = -1;
		}
		if (rcv == -1)
		{
			throw std::runtime_error("Cannot receive server answer");
		}
	}
	
	
	
	switch (msg.status)
	{
		case db_ipc::command_status::OK:
			break;
		case db_ipc::command_status::BAD_ALLOC:
			throw std::runtime_error("Memory lack occured");
		case db_ipc::command_status::INVALID_STRUCT_NAME:
			throw std::runtime_error("Invalid struct name");
		case db_ipc::command_status::TOO_BIG_STRUCT_NAME:
			throw std::runtime_error("Too big struct name");
		case db_ipc::command_status::INVALID_PATH:
			throw std::runtime_error("Invalid structs path");
		case db_ipc::command_status::STRUCT_DOES_NOT_EXIST:
			throw std::runtime_error("Enteted struct does not exist");
		case db_ipc::command_status::POOL_DOES_NOT_EXIST:
			throw std::runtime_error("Enteted pool does not exist");
		case db_ipc::command_status::SCHEMA_DOES_NOT_EXIST:
			throw std::runtime_error("Entered schema does not exist");
		case db_ipc::command_status::COLLECTION_DOES_NOT_EXIST:
			throw std::runtime_error("Entered collection does not exist");
		case db_ipc::command_status::ATTEMPT_TO_ADD_POOL_DUBLICATE:
			throw std::runtime_error("Entered pool already exists");
		case db_ipc::command_status::ATTEMPT_TO_ADD_SCHEMA_DUBLICATE:
			throw std::runtime_error("Entered schema already exists");
		case db_ipc::command_status::ATTEMPT_TO_ADD_COLLECTION_DUBLICATE:
			throw std::runtime_error("Entered collection already exists");
		case db_ipc::command_status::ATTEMPT_TO_INSERT_EXISTENT_KEY:
			throw std::runtime_error("Entered key already exists");
		case db_ipc::command_status::ATTEMPT_TO_UPDATE_NONEXISTENT_KEY:
			throw std::runtime_error("Entered key does not exist");
		case db_ipc::command_status::ATTEMPT_TO_DISPOSE_NONEXISTENT_KEY:
			throw std::runtime_error("Entered key does not exist");
		case db_ipc::command_status::ATTTEMT_TO_OBTAIN_NONEXISTENT_KEY:
			switch (msg.cmd)
			{
				case db_ipc::command::OBTAIN_BETWEEN:
					throw std::runtime_error("Entered keys do not exist");
				default:
					throw std::runtime_error("Entered key does not exist");
			}
		case db_ipc::command_status::FAILED_TO_ADD_STRUCT:
			switch (msg.cmd)
			{
				case db_ipc::command::ADD_POOL:
					throw std::runtime_error("Failed to add pool");
				case db_ipc::command::ADD_SCHEMA:
					throw std::runtime_error("Failed to add schema");
				case db_ipc::command::ADD_COLLECTION:
					throw std::runtime_error("Failed to add collection");
				default:
					throw std::runtime_error("Failed to add struct");
			}
		case db_ipc::command_status::FAILED_TO_DISPOSE_STRUCT:
			switch (msg.cmd)
			{
				case db_ipc::command::ADD_POOL:
					throw std::runtime_error("Failed to dispose pool");
				case db_ipc::command::ADD_SCHEMA:
					throw std::runtime_error("Failed to dispose schema");
				case db_ipc::command::ADD_COLLECTION:
					throw std::runtime_error("Failed to dispose collection");
				default:
					throw std::runtime_error("Failed to dispose struct");
			}
		case db_ipc::command_status::FAILED_TO_PERFORM_DATA_COMMAND:
			switch (msg.cmd)
			{
				case db_ipc::command::ADD:
					throw std::runtime_error("Failed to insert key");
				case db_ipc::command::UPDATE:
					throw std::runtime_error("Failed to update key");
				case db_ipc::command::DISPOSE:
					throw std::runtime_error("Failed to dispose key");
				case db_ipc::command::OBTAIN:
					throw std::runtime_error("Failed to obtain key");
				default:
					throw std::runtime_error("Failed to perform data command");
			}
		case db_ipc::command_status::FAILED_TO_INSERT_KEY:
			throw std::runtime_error("Failed to insert key");
		case db_ipc::command_status::FAILED_TO_UPDATE_KEY:
			throw std::runtime_error("Failed to update key");
		case db_ipc::command_status::FAILED_TO_DISPOSE_KEY:
			throw std::runtime_error("Failed to dispose key");
		case db_ipc::command_status::FAILED_TO_OBTAIN_KEY:
			throw std::runtime_error("Failed to obtain key");
	}
	
	switch (msg.cmd)
	{
		case db_ipc::command::ADD:
			std::cout << "Added record { " << msg.login << " : " << msg.hashed_password << ", " << msg.name << " }." << std::endl;
			break;
		case db_ipc::command::UPDATE:
			std::cout << "Updated record { " << msg.login << " : " << msg.hashed_password << ", " << msg.name << " }." << std::endl;
			break;
		case db_ipc::command::DISPOSE:
			std::cout << "Disposed record { " << msg.login << " : " << msg.hashed_password << ", " << msg.name << " }." << std::endl;
			break;
		case db_ipc::command::OBTAIN:
			std::cout << "Obtained record { " << msg.login << " : " << msg.hashed_password << ", " << msg.name << " }." << std::endl;
			break;
		case db_ipc::command::OBTAIN_BETWEEN:
			{
				size_t counter = msg.status == db_ipc::command_status::OBTAIN_BETWEEN_END ? 1 : 0;
				size_t target = msg.extra_value;
				
				std::cout << "Between " << login << " and " << msg.right_boundary_login << std::endl <<
					"Obtained record { " << msg.login << " : " << msg.hashed_password << ", " << msg.name << " }." << std::endl;
				
				while(counter != target)
				{
					rcv = msgrcvt(25, mq_descriptor, msg, db_ipc::MANAGER_SERVER_MSG_SIZE, getpid());
					
					if (rcv == -1)
					{
						throw std::runtime_error("Cannot receive server answer");
					}
					
					std::cout << "Obtained record { " << msg.login << " : " << msg.hashed_password << ", " << msg.name << " }." << std::endl;
					
					if (msg.status == db_ipc::command_status::OBTAIN_BETWEEN_END) ++counter;
				
				} while (counter != target);
			}
			break;
		case db_ipc::command::ADD_POOL:
			std::cout << "Added pool '" << msg.pool_name << "'" << std::endl;
			break;
		case db_ipc::command::ADD_SCHEMA:
			std::cout << "Added schema '" << msg.pool_name << '/' << msg.schema_name << "'" << std::endl;
			break;
		case db_ipc::command::ADD_COLLECTION:
			std::cout << "Added collection '" << msg.pool_name << '/' << msg.schema_name << '/' << msg.collection_name << "'" << std::endl;
			break;
		case db_ipc::command::DISPOSE_POOL:
			std::cout << "Disposed schema '" << msg.pool_name << "'" << std::endl;
			break;
		case db_ipc::command::DISPOSE_SCHEMA:
			std::cout << "Disposed schema '" << msg.pool_name << '/' << msg.schema_name << "'" << std::endl;
			break;
		case db_ipc::command::DISPOSE_COLLECTION:
			std::cout << "Disposed pool '" << msg.pool_name << '/' << msg.schema_name << '/' << msg.collection_name << "'" << std::endl;
			break;
		default:
			throw std::runtime_error("Invalid server answer");
	}
	
	std::cout << std::endl;
}

int msgrcvt(
    int max_counter,
    int descriptor,
    db_ipc::strg_msg_t &msg,
    size_t msg_size,
    long mtypes,
    int flags)
{
    ssize_t rcv = -1;
    int counter = 0;
    do
    {
        rcv = msgrcv(descriptor, &msg, msg_size, mtypes, flags | IPC_NOWAIT);
		
		if (counter < 3)
		{
			usleep(250);
		}
		else
		{
			sleep(2);
		}
		
		// if (rcv == -1)
		// {
		// 	if (counter < 5)
		// 	{
		// 		usleep(250);
		// 	}
		// 	else
		// 	{
		// 		msg.cmd = db_ipc::command::PING;
		// 		msgsnd(descriptor, &msg, db_ipc::MANAGER_SERVER_MSG_SIZE, MSG_NOERROR);
		// 	}
		// }
		// else if (msg.cmd == db_ipc::command::PING)
		// {
		// 	if (msg.status == db_ipc::command_status::SERVER_IS_BUSY)
		// 	{
		// 		std::cout << "Server is busy. Please, wait" << std::endl;
		// 		sleep(10);
		// 		counter = 0;
		// 	}
			
		// 	rcv = -1;
		// 	msg.status = db_ipc::command_status::OK;	
		// }
		
    } while (++counter < max_counter && rcv == -1);
    
    return rcv;
}

void run_session(
	int mq_descriptor,
    std::istream &stream,
	bool is_terminal,
	size_t &request_counter)
{
    std::string buf;
    std::string cmd;
	
	bool error = false;
	std::string err_msg;

    std::string key;
    tvalue value;
	
	db_ipc::strg_msg_t msg;

    while(std::getline(stream, buf))
    {
		if (buf[0] == '\0')
		{
			continue;
		}
		
		msg.req_id = request_counter;
		
        std::istringstream line(buf);
        line >> cmd;

		try
		{
			if (cmd == "stop")
			{
				validate_eof(line);
				break;
			}
			else if (cmd == "add")
			{
				handle_add_command(mq_descriptor, line, msg);
			}
			else if (cmd == "update")
			{
				handle_update_command(mq_descriptor, line, msg);
			}
			else if (cmd == "dispose")
			{
				handle_dispose_command(mq_descriptor, line, msg);
			}
			else if (cmd == "obtain")
			{
				handle_obtain_command(mq_descriptor, line, msg);
			}
			else if (cmd == "addPool")
			{
				handle_add_pool_command(mq_descriptor, line, msg);
			}
			else if (cmd == "addSchema")
			{
				handle_add_schema_command(mq_descriptor, line, msg);
			}
			else if (cmd == "addCollection")
			{
				handle_add_collection_command(mq_descriptor, line, msg);
			}
			else if (cmd == "disposePool")
			{
				handle_dispose_pool_command(mq_descriptor, line, msg);
			}
			else if (cmd == "disposeSchema")
			{
				handle_dispose_schema_command(mq_descriptor, line, msg);
			}
			else if (cmd == "disposeCollection")
			{
				handle_dispose_collection_command(mq_descriptor, line, msg);
			}
			else if (cmd == "executeFile")
			{
				handle_execute_file_command(mq_descriptor, line, request_counter);
				request_counter = request_counter % static_cast<size_t>(1e9) + 1;
				continue;
			}
			else if (cmd == "reconnect")
			{
				handle_reconnect_command(mq_descriptor, line);
				continue;
			}
			else
			{
				throw std::runtime_error("Invalid command");
			}
			
			handle_server_answer(mq_descriptor, msg, request_counter);
			request_counter = request_counter % static_cast<size_t>(1e9) + 1;
		}
		catch (std::runtime_error const &ex)
		{
			if (is_terminal)
			{
				std::cout << ex.what() << std::endl << std::endl;
			}
			else
			{
				throw;
			}
		}
    }
}
