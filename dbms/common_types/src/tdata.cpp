#include <cstring>
#include <iostream>
#include <fstream>

#include "../include/tdata.h"

int tkey_comparer::operator()(
        tkey const &lhs,
        tkey const &rhs) const
{

    if (!lhs or !rhs) {
        if (lhs == rhs) {
            return 0;
        }
        return lhs ? 1 : -1;
    }

    if (lhs->get_data() < rhs->get_data()) {
        return -1;
    }

    if (lhs->get_data() > rhs->get_data()) {
        return 1;
    }
    return 0;
}

int tkey_comparer::operator()(
 	std::string const &lhs,
    std::string const &rhs) const
 {
 	if (lhs != rhs)
 	{
 		return lhs < rhs ? -1 : 1;
 	}
 	return 0;
 }

tvalue::tvalue()
        : personal_id(0),
          name(flyweight_factory::get_instance()->get_flyweight_instance(""))
{ }

tvalue::tvalue(
        uint64_t hashed_password,
        std::string const &name)
        : personal_id(hashed_password),
          name(flyweight_factory::get_instance()->get_flyweight_instance(name))
{ }

tvalue::tvalue(uint64_t hashed_password, std::shared_ptr<flyweight_string> name)
        : personal_id(hashed_password),
          name(name)
{ }


// tvalue::~tvalue()
// {
// 	delete[] name;
// }

// tvalue::tvalue(
// 	tvalue const &other):
// 		personal_id(other.personal_id),
// 		name(new char[strlen(other.name) + 1])
// {
// 	strcpy(name, other.name);
// }

// tvalue tvalue::operator=(
// 	tvalue const &other)
// {
// 	if (this != &other)
// 	{
// 		delete[] name;
// 		name = (new char[strlen(other.name) + 1]);
// 		strcpy(name, other.name);
// 	}
	
// 	return *this;
// }

// tvalue::tvalue(
// 	tvalue &&other) noexcept:
// 		personal_id(other.personal_id),
// 		name(other.name)
// {
// 	other.name = nullptr;
// }

// tvalue tvalue::operator=(
// 	tvalue &&other) noexcept
// {
// 	if (this != &other)
// 	{
// 		personal_id = other.personal_id;
// 		name = other.name;
		
// 		other.name = nullptr;
// 	}
	
// 	return *this;
// }

ram_tdata::ram_tdata(
	tvalue const &value):
		value(value)
{ }

ram_tdata::ram_tdata(
	tvalue &&value):
		value(std::move(value))
{ }


file_tdata::file_tdata(
	long file_pos):
		_file_pos(file_pos)
{ }

void file_tdata::serialize(
	std::string const &path,
	tkey const &key,
	tvalue const &value,
	bool update_flag)
{
	std::ofstream data_stream(path, std::ios::app | std::ios::binary);
	
    if (!data_stream.is_open())
    {
        throw std::ios::failure("Cannot open the file");
    }
	
	// TODO UPDATE WITH BIGGER DATA
	
	if (_file_pos == -1 || update_flag)
	{
		_file_pos = data_stream.tellp();
	}
	else
	{
		data_stream.seekp(_file_pos, std::ios::beg);
	}

    std::string key_str = key->get_data();
    std::string name_str = value.name->get_data();
    size_t login_len = key_str.size();
    size_t name_len = name_str.size();
	
    data_stream.write(reinterpret_cast<char const *>(&login_len), sizeof(size_t));
    data_stream.write(key->get_data().c_str(), sizeof(char) * login_len);
    data_stream.write(reinterpret_cast<char const *>(&value.personal_id), sizeof(int64_t));
    data_stream.write(reinterpret_cast<char const *>(&name_len), sizeof(size_t));
    data_stream.write(value.name->get_data().c_str(), sizeof(char) * name_len);
	data_stream.flush();
	
	if (data_stream.fail())
	{
		throw std::ios::failure("An error occured while serializing data");
	}
}

tvalue file_tdata::deserialize(
	std::string const &path) const
{
	std::ifstream data_stream(path, std::ios::binary);
    if (!data_stream.is_open())
    {
        throw std::ios::failure("Cannot open the file");
    }
	if (_file_pos == 1)
	{
		throw std::logic_error("Invalid pointer to data");
	}

    data_stream.seekg(_file_pos, std::ios::beg);
	
	tvalue value;
	size_t login_len, name_len;
	
	data_stream.read(reinterpret_cast<char *>(&login_len), sizeof(size_t));
	data_stream.seekg(login_len, std::ios::cur);
    data_stream.read(reinterpret_cast<char *>(&value.personal_id), sizeof(size_t));
	data_stream.read(reinterpret_cast<char *>(&name_len), sizeof(size_t));

    std::string name_str(name_len, '\0');
    data_stream.read(&name_str[0], sizeof(char) * name_len);

    value.name = flyweight_factory::get_instance()->get_flyweight_instance(name_str);
	
	if (data_stream.fail())
	{
		throw std::ios::failure("An error occured while deserializing data");
	}
	
    return value;
}
