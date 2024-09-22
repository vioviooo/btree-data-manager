#ifndef MATH_PRACTICE_AND_OPERATING_SYSTEMS_COMMON_EXTRA_UTILITY_H
#define MATH_PRACTICE_AND_OPERATING_SYSTEMS_COMMON_EXTRA_UTILITY_H

#include <string>
#include "../../dbms/common_types/include/flyweight.h"

namespace extra_utility
{
    
    template<
        typename T>
    std::string make_string(T const &value);
    
    template<> std::string make_string(int const &value);
    template<> std::string make_string(long const &value);
    template<> std::string make_string(long long const &value);
    template<> std::string make_string(unsigned const &value);
    template<> std::string make_string(unsigned long const &value);
    template<> std::string make_string(unsigned long long const &value);
    template<> std::string make_string(float const &value);
    template<> std::string make_string(double const &value);
    template<> std::string make_string(long double const &value);
    template<> std::string make_string(std::string const &value);
    template<> std::string make_string(char * const &value);
    template<> std::string make_string(std::shared_ptr<flyweight_string> const &value);
    
    std::string make_path(
	    std::initializer_list<std::string> list);
    
    std::string make_path(
	    std::initializer_list<char const *> list);
    
}

#endif //MATH_PRACTICE_AND_OPERATING_SYSTEMS_COMMON_EXTRA_UTILITY_H