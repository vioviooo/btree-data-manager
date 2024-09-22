#include <string>
#include <sstream>
#include <iomanip>

#include <not_implemented.h>

#include "../include/allocator_global_heap.h"

allocator_global_heap::allocator_global_heap(
    logger *logger):
    _logger(logger)
{
    trace_with_guard(get_typename() + "::allocator_global_heap(logger *) : called.");
    
    trace_with_guard(get_typename() + "::allocator_global_heap(logger *) : successfuly finished.");
}

allocator_global_heap::~allocator_global_heap()
{
    trace_with_guard(get_typename() + "::~allocator_global_heap() : called.");
    
    trace_with_guard(get_typename() + "::~allocator_global_heap() : successfuly finished.");
}

allocator_global_heap::allocator_global_heap(
    allocator_global_heap &&other) noexcept:
    _logger(other._logger)
{
    trace_with_guard(get_typename() + "::allocator_global_heap(allocator_global_heap &&) : called.");
    
    trace_with_guard(get_typename() + "::allocator_global_heap(allocator_global_heap &&) : successfuly finished.");
}

allocator_global_heap &allocator_global_heap::operator=(
    allocator_global_heap &&other) noexcept
{
    trace_with_guard(get_typename() + "::operator=(allocator_global_heap &&) : called.");
    
    if (this != &other)
    {
        _logger = other._logger;
        other._logger = nullptr;
    }
    
    trace_with_guard(get_typename() + "::operator=(allocator_global_heap &&) : successfuly finished.");
    
    return *this;
}

[[nodiscard]] void *allocator_global_heap::allocate(
    size_t value_size,
    size_t values_count)
{
    trace_with_guard(get_typename() + "::allocate(size_t, size_t) : called.")
        ->debug_with_guard(get_typename() + "::allocate(size_t, size_t) : called. (value size = " +
            std::to_string(value_size) + "; count = " + std::to_string(values_count) + ").");
    
    unsigned char *ptr = nullptr;
    size_t size = value_size * values_count;
    
    try
    {
        ptr = reinterpret_cast<unsigned char*>(
                ::operator new(size + sizeof(allocator_global_heap*) + sizeof(size_t)));
    }
    catch(std::bad_alloc const &)
    {
        error_with_guard(get_typename() + "::allocate(size_t, size_t) : " +
                "bad alloc occurred while trying to allocate " + std::to_string(size) + " bytes.");
        throw;
    }
    
    *reinterpret_cast<allocator_global_heap**>(ptr) = this;
    *reinterpret_cast<size_t*>(ptr + sizeof(allocator_global_heap*)) = size;
    
    trace_with_guard(get_typename() + "::allocate(size_t, size_t) : successfuly finished.")
        ->debug_with_guard(get_typename() + "::allocate(size_t, size_t) : successfuly finished.");
    
    return reinterpret_cast<void*>(ptr + sizeof(allocator_global_heap*) + sizeof(size_t));
}

void allocator_global_heap::deallocate(
    void *at)
{
    trace_with_guard(get_typename() + "::deallocate(void *) : called.")
        ->debug_with_guard(get_typename() + "::deallocate(void *) : called.");
    
    auto ptr = reinterpret_cast<unsigned char*>(at);
    
    ptr -= sizeof(size_t);
    size_t size = *reinterpret_cast<size_t*>(ptr);
    
    ptr -= sizeof(allocator_global_heap*);
    allocator_global_heap* allctr_ptr = *reinterpret_cast<allocator_global_heap**>(ptr);
    
    if (allctr_ptr != this)
    {
        error_with_guard(get_typename() + "::deallocate(void *) : tried to deallocate non-related memory.");
        throw std::logic_error("try of deallocation non-related memory");
    }
    
    std::string dump = get_block_dump(at, size);
    
    ::operator delete(ptr);
    
    debug_with_guard(get_typename() + "::deallocate(void *): deallocated " +
            std::to_string(size) + " bytes" + (!dump.size() ? "" : " with data " + dump) + ".")
        ->trace_with_guard(get_typename() + "::deallocate(void *) : successfuly finished.")
        ->debug_with_guard(get_typename() + "::deallocate(void *) : successfuly finished.");
}

inline logger *allocator_global_heap::get_logger() const
{
    return _logger;
}

inline std::string allocator_global_heap::get_typename() const noexcept
{
    return "allocator_global_heap";
}
