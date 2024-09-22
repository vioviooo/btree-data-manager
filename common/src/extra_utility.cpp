#include <type_traits>

#include <file_cannot_be_opened.h>

#include "../include/extra_utility.h"

template<
    typename T>
std::string extra_utility::make_string(T const &value)
{
    return "unimplemented parse";
}

template<> std::string extra_utility::make_string(int const &value)                 { return std::to_string(value); }
template<> std::string extra_utility::make_string(long const &value)                { return std::to_string(value); }
template<> std::string extra_utility::make_string(long long const &value)           { return std::to_string(value); }
template<> std::string extra_utility::make_string(unsigned const &value)            { return std::to_string(value); }
template<> std::string extra_utility::make_string(unsigned long const &value)       { return std::to_string(value); }
template<> std::string extra_utility::make_string(unsigned long long const &value)  { return std::to_string(value); }
template<> std::string extra_utility::make_string(float const &value)               { return std::to_string(value); }
template<> std::string extra_utility::make_string(double const &value)              { return std::to_string(value); }
template<> std::string extra_utility::make_string(long double const &value)         { return std::to_string(value); }
template<> std::string extra_utility::make_string(std::string const &value)         { return value; }
template<> std::string extra_utility::make_string(char * const &value)              { return std::string(value); }
template<> std::string extra_utility::make_string(std::shared_ptr<flyweight_string> const &value)  { return value->get_data(); }

std::string extra_utility::make_path(
	std::initializer_list<std::string> list)
{
	std::string path = "";
		
	for (auto elem : list)
	{
		if (path[0])
		{
			path.push_back('/');
		}
		path += elem;
	}
	
	return path;
}

std::string extra_utility::make_path(
	std::initializer_list<char const *> list)
{
	std::string path = "";
		
	for (auto elem : list)
	{
		if (!path.empty())
		{
			path.push_back('/');
		}
		path += elem;
	}
	
	return path;
}