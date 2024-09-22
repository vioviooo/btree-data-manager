#ifndef MATH_PRACTICE_AND_OPERATING_SYSTEMS_ASSOCIATIVE_CONTAINER_H
#define MATH_PRACTICE_AND_OPERATING_SYSTEMS_ASSOCIATIVE_CONTAINER_H

#include <iostream>
#include <vector>
#include <operation_not_supported.h>

template<
    typename tkey,
    typename tvalue>
class associative_container
{

protected:

    class default_key_comparer final
    {

    public:
        
        int operator()(
            int first,
            int second);
        
        int operator()(
            std::string const &first,
            std::string const &second);
        
    };

public:
    
    struct key_value_pair final
    {
    
    public:
        
        tkey key;
        tvalue value;
    
    public:
    
        key_value_pair(
            tkey const &key,
            tvalue const &value);
        
        key_value_pair(
            tkey const &key,
            tvalue&& value);
    
    };
    
    struct key_value_ptr_pair
    {
    
    public:
        
        tkey key;
        tvalue *value_ptr;
        
    };

public:
    
    // TODO
    struct associative_container_iterator
    {
    
    public:
    
    };

public:
    
    virtual ~associative_container() noexcept = default;

public:
    
    virtual void insert(
        tkey const &key,
        tvalue const &value) = 0;
    
    virtual void insert(
        tkey const &key,
        tvalue &&value) = 0;
    
    virtual void update(
        tkey const &key,
        tvalue const &value) = 0;
    
    virtual void update(
        tkey const &key,
        tvalue &&value) = 0;
    
    virtual tvalue &obtain(
        tkey const &key) = 0;
    
    virtual tvalue dispose(
        tkey const &key) = 0;
  
};

template<
    typename tkey,
    typename tvalue>
int associative_container<tkey, tvalue>::default_key_comparer::operator()(
    int first,
    int second)
{
    return first - second;
}

template<
    typename tkey,
    typename tvalue>
int associative_container<tkey, tvalue>::default_key_comparer::operator()(
    std::string const &first,
    std::string const &second)
{
    if (first > second)
    {
        return 1;
    }
    else if (first == second)
    {
        return 0;
    }
    return -1;
}

template<
    typename tkey,
    typename tvalue>
associative_container<tkey, tvalue>::key_value_pair::key_value_pair(
    tkey const &key,
    tvalue const &value):
        key(key), value(value)
{ }

template<
    typename tkey,
    typename tvalue>
associative_container<tkey, tvalue>::key_value_pair::key_value_pair(
    tkey const &key,
    tvalue&& value):
        key(key), value(std::move(value))
{ }

#endif //MATH_PRACTICE_AND_OPERATING_SYSTEMS_ASSOCIATIVE_CONTAINER_H