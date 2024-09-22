#ifndef OPERATING_SYSTEMS_COURSE_WORK_DATABASE_MANAGEMENT_SYSTEM_COMMON_TYPES_IPC_DATA
#define OPERATING_SYSTEMS_COURSE_WORK_DATABASE_MANAGEMENT_SYSTEM_COMMON_TYPES_IPC_DATA

#include <unistd.h>
#include <sys/types.h>

namespace db_ipc
{
	
	enum class search_tree_variant
	{
		B,
		B_PLUS,
		B_STAR,
		B_STAR_PLUS,
	};
	
	enum class allocator_variant
	{
		GLOBAL_HEAP,
		SORTED_LIST,
		BUDDY_SYSTEM,
		BOUNDARY_TAGS,
		RED_BLACK_TREE
	};
	
	enum class allocator_fit_mode
    {
		FIRST_FIT,
		THE_BEST_FIT,
		THE_WORST_FIT,
    };
	
	enum class command
	{
		// manage commands
		
		ADD_STORAGE_SERVER,
		TERMINATE_STORAGE_SERVER,
		
		SET_IN_MEMORY_CACHE_MODE,
		SET_FILE_SYSTEM_MODE,
		END_SETUP, // delete?
		
		PING,
		SHUTDOWN,
		TERMINATE,
		TERMINATE_ASSISTANT_THREAD,
		
		GET_RECORDS_CNT,
		
		// data commands
		
		ADD_POOL,
		ADD_SCHEMA,
		ADD_COLLECTION,
		DISPOSE_POOL,
		DISPOSE_SCHEMA,
		DISPOSE_COLLECTION,
		
		ADD,
		UPDATE,
		DISPOSE,
		OBTAIN,
		OBTAIN_BETWEEN,
		OBTAIN_MIN,
		OBTAIN_MAX,
		OBTAIN_NEXT,
	};
	
	enum class command_status
	{
		CLIENT,
		SERVER_IS_BUSY,
		
		OK,
		OBTAIN_BETWEEN_END,
		
		BAD_ALLOC,
		
		INVALID_STRUCT_NAME,
		TOO_BIG_STRUCT_NAME,
		INVALID_PATH,
		
		ATTEMPT_TO_CHANGE_SETUP,
		
		STRUCT_DOES_NOT_EXIST,
		POOL_DOES_NOT_EXIST,
		SCHEMA_DOES_NOT_EXIST,
		COLLECTION_DOES_NOT_EXIST,
		
		ATTEMPT_TO_ADD_STRUCT_DUBLICATE,
		ATTEMPT_TO_ADD_POOL_DUBLICATE,
		ATTEMPT_TO_ADD_SCHEMA_DUBLICATE,
		ATTEMPT_TO_ADD_COLLECTION_DUBLICATE,
		
		ATTEMPT_TO_INSERT_EXISTENT_KEY,
		ATTEMPT_TO_UPDATE_NONEXISTENT_KEY,
		ATTEMPT_TO_DISPOSE_NONEXISTENT_KEY,
		ATTTEMT_TO_OBTAIN_NONEXISTENT_KEY,
		
		FAILED_TO_SETUP_STORAGE_SERVER,
		FAILED_TO_ADD_STRUCT,
		FAILED_TO_DISPOSE_STRUCT,
		FAILED_TO_PERFORM_DATA_COMMAND,
		FAILED_TO_INSERT_KEY,
		FAILED_TO_UPDATE_KEY,
		FAILED_TO_DISPOSE_KEY,
		FAILED_TO_OBTAIN_KEY,
	};
	
	int constexpr MSG_KEY_SIZE = 64;
	int constexpr MSG_NAME_SIZE = 64;
	int constexpr MSG_STRUCTS_NAME_SIZE = 255;

	struct strg_msg_t
	{
		// metadata
		
		long mtype;
		pid_t pid;
		size_t req_id;
		command cmd;
		command_status status;
		size_t extra_value;
		
		// path
		
		char pool_name[MSG_STRUCTS_NAME_SIZE];
		char schema_name[MSG_STRUCTS_NAME_SIZE];
		char collection_name[MSG_STRUCTS_NAME_SIZE];
		
		// data
		
		search_tree_variant tree_variant;
		allocator_variant alloc_variant;
		allocator_fit_mode alloc_fit_mode;
		size_t t_for_b_trees;
		
		char login[MSG_KEY_SIZE];
		char right_boundary_login[MSG_KEY_SIZE];
		int64_t hashed_password;
		char name[MSG_NAME_SIZE];
	};
	
	

	int constexpr MANAGER_SERVER_MQ_KEY = 50;
	int constexpr MANAGER_SERVER_MSG_SIZE = sizeof(strg_msg_t) - sizeof(long);
	
	int constexpr MANAGER_SERVER_MAX_COMMON_PRIOR = 30;
	int constexpr MANAGER_SERVER_MAX_COMMAND_PRIOR = 50;
	
	
	int constexpr MANAGER_SERVER_STORAGE_ANSWER_PRIOR = 8;
	int constexpr MANAGER_SERVER_CLIENT_PRIOR = 10;
	int constexpr MANAGER_SERVER_CLIENT_PING_PRIOR = 11;
	int constexpr MANAGER_SERVER_STORAGE_ADDITION_PRIOR = 31;
	int constexpr MANAGER_SERVER_STRUCT_ADDITION_PRIOR = 32;
	int constexpr MANAGER_SERVER_STRUCT_DISPOSAL_PRIOR = 33;
	int constexpr MANAGER_SERVER_STORAGE_GETTING_RECORDS_CNT_PRIOR = 40;
	
}

#endif //OPERATING_SYSTEMS_COURSE_WORK_DATABASE_MANAGEMENT_SYSTEM_COMMON_TYPES_IPC_DATA