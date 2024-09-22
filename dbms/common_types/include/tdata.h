#ifndef OPERATING_SYSTEMS_COURSE_WORK_DATABASE_MANAGEMENT_SYSTEM_COMMON_TYPES_TDATA
#define OPERATING_SYSTEMS_COURSE_WORK_DATABASE_MANAGEMENT_SYSTEM_COMMON_TYPES_TDATA

#include <allocator.h>
#include "flyweight.h"

using tkey = std::shared_ptr<flyweight_string>;

class tkey_comparer final
{

public:

    int operator()(
        tkey const &lhs,
        tkey const &rhs) const;

    int operator()(
            std::string const &lhs,
            std::string const &rhs) const;

};

class tvalue final
{

public:

	uint64_t personal_id;
	std::shared_ptr<flyweight_string> name;

public:
	
	tvalue();
	
	tvalue(
		uint64_t hashed_password,
		std::string const &name);

    tvalue(
            uint64_t hashed_password,
    std::shared_ptr<flyweight_string> name);
};

class tdata
{

public:

	virtual ~tdata() = default;

};

class ram_tdata final
	: public tdata
{

public:

	tvalue value;

public:

	ram_tdata(
		tvalue const &value);

	ram_tdata(
		tvalue &&value);

};

class file_tdata final
	: public tdata
{

private:

	long _file_pos;

public:

	file_tdata(
		long file_pos = -1);
	
	void serialize(
		std::string const &path,
		tkey const &key,
		tvalue const &value,
		bool update_flag = false);
	
	tvalue deserialize(
		std::string const &path) const;

};

#endif //OPERATING_SYSTEMS_COURSE_WORK_DATABASE_MANAGEMENT_SYSTEM_COMMON_TYPES_TDATA