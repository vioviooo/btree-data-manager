#include "../include/allocator_buddies_system.h"
#include <mutex>
#include <string>
#include <iomanip>
#include <sstream>

allocator_buddies_system::~allocator_buddies_system()
{
    if (get_logger() != nullptr)
    {
        trace_with_guard(get_typename() + "::~allocator_buddies_system() was called");
    }
    get_mutex().~mutex();
    if (get_logger() != nullptr)
    {
        trace_with_guard(get_typename() + "::~allocator_buddies_system() finished");
    }
    deallocate_with_guard(_trusted_memory);
}

allocator_buddies_system::allocator_buddies_system(
        allocator_buddies_system &&other) noexcept : _trusted_memory(other._trusted_memory)
{
    trace_with_guard(get_typename() + "::allocator_buddies_system(allocator_buddies_system &&other) was called");
    other._trusted_memory = nullptr;
    trace_with_guard(get_typename() + "::allocator_buddies_system(allocator_buddies_system &&other) finished");
}

allocator_buddies_system &allocator_buddies_system::operator=(
        allocator_buddies_system &&other) noexcept
{
    trace_with_guard(get_typename() + "operator=(allocator_buddies_system &&other) was called");
    if (this == &other)
    {
        return *this;
    }
    get_mutex().~mutex();
    deallocate_with_guard(_trusted_memory);
    _trusted_memory = other._trusted_memory;
    other._trusted_memory = nullptr;
    trace_with_guard(get_typename() + "operator=(allocator_buddies_system &&other) finished");
    return *this;
}

allocator_buddies_system::allocator_buddies_system(
        size_t space_size,
        allocator *parent_allocator,
        logger *logger,
        allocator_with_fit_mode::fit_mode allocate_fit_mode)
{
    if (logger != nullptr)
    {
        logger->trace(get_typename() + "::allocator_buddies_system (size_t space_size, allocator *parent_allocator, logger *logger, allocator_with_fit_mode::fit_mode allocate_fit_mode) was called");
    }
    block_size_t memory_size = 1ull << space_size;
    if (memory_size < get_available_block_meta_size())
    {
        if (logger != nullptr)
        {
            logger->error("Cannot initialize allocator: size of:" +  std::to_string(space_size) + "is too small. Minimum size is:" + std::to_string(get_available_block_meta_size()));
        }
        throw std::logic_error("Cannot initialize allocator");
    }
    block_size_t size_to_alloc = memory_size + get_allocator_meta_size();
    try
    {
        _trusted_memory = parent_allocator == nullptr
                          ? ::operator new(size_to_alloc)
                          : parent_allocator->allocate(size_to_alloc, 1);
    }
    catch (std::bad_alloc const &err)
    {
        throw err;
    }

    auto* temp_pointer = reinterpret_cast<unsigned char*>(_trusted_memory);

    *reinterpret_cast<allocator**>(temp_pointer) = parent_allocator;
    temp_pointer += sizeof(allocator*);

    *reinterpret_cast<class logger**>(temp_pointer) = logger;
    temp_pointer += sizeof(class logger*);

    new(reinterpret_cast<std::mutex*>(temp_pointer)) std::mutex();
    temp_pointer += sizeof(std::mutex);

    *reinterpret_cast<fit_mode*>(temp_pointer) = allocate_fit_mode;
    temp_pointer += sizeof(fit_mode);

    *reinterpret_cast<unsigned char*>(temp_pointer) = space_size;
    temp_pointer += sizeof(unsigned char);

    *reinterpret_cast<block_size_t *>(temp_pointer) = memory_size;
    temp_pointer += sizeof(block_size_t);

    block_pointer_t first_block = reinterpret_cast<unsigned char*>(_trusted_memory) + get_allocator_meta_size();
    *reinterpret_cast<block_pointer_t*>(temp_pointer) = first_block;
    temp_pointer += sizeof(block_pointer_t);

    *reinterpret_cast<unsigned char*>(temp_pointer) = space_size;
    avail_block(temp_pointer);
    temp_pointer += sizeof (unsigned char );

    *reinterpret_cast<block_pointer_t*>(temp_pointer) = nullptr;
    temp_pointer += sizeof (block_pointer_t);

    *reinterpret_cast<block_pointer_t*>(temp_pointer) = nullptr;

    trace_with_guard(get_typename() + "::allocator_buddies_system (size_t space_size, allocator *parent_allocator, logger *logger, allocator_with_fit_mode::fit_mode allocate_fit_mode) finished");
}

[[nodiscard]] void *allocator_buddies_system::allocate(
        size_t value_size,
        size_t values_count)
{
    std::lock_guard<std::mutex> guard(get_mutex());
    debug_with_guard(get_typename() + "::allocate(size_t value_size, size_t values_count) was called");

    block_size_t requested_size = value_size * values_count;
    block_size_t block_meta_size = get_available_block_meta_size();
    unsigned char power_of_allocation_size = get_power_of_size(requested_size + block_meta_size);
    unsigned char power_of_requested_size = get_power_of_size(requested_size);

    block_pointer_t target_prev = nullptr;
    block_pointer_t target_block = nullptr;
    block_pointer_t target_next = nullptr;
    block_size_t target_size = 0;

    block_pointer_t &first_block = get_first_available_block();
    block_pointer_t current_block = first_block;
    block_pointer_t previous_block = nullptr;

    fit_mode fit = get_fit_mode();

    unsigned char size = current_block == nullptr
                         ? get_allocator_data_size()
                         : get_block_data_size(current_block);
    if (size >= power_of_allocation_size)
    {
        target_block = current_block;
        target_size = size;
    }

    while (current_block != nullptr)
    {
        if (fit == fit_mode::first_fit && target_block)
        {
            break;
        }
        size = get_block_data_size(current_block);


        if ((size >= power_of_allocation_size) && (target_block == nullptr || (fit == fit_mode::the_best_fit && (size - power_of_requested_size < target_size - power_of_requested_size))
                                                   || (fit == fit_mode::the_worst_fit && (size - power_of_requested_size > target_size - power_of_requested_size))))
        {
            target_prev = previous_block;
            target_block = current_block;
            target_size = size;
        }
        previous_block = current_block;
        current_block = get_next_available_block(current_block);
    }
    if (target_block == nullptr)
    {
        error_with_guard(get_typename() + "There is no space to allocate memory to allocate" + std::to_string(requested_size) + "bytes");
        throw std::bad_alloc();
    }
    block_pointer_t buddy = nullptr;

    while (((1 << target_size) >> 1) >= (1 << power_of_allocation_size))
    {
        target_size--;
        set_block_size(target_block) = target_size;
        buddy = get_buddy(target_block);
        set_block_size(buddy) = target_size;
        avail_block(buddy);
        get_next_available_block(buddy) = get_next_available_block(target_block);
        get_next_available_block(target_block) = buddy;
        block_pointer_t temp_pointer = get_next_available_block(get_next_available_block(target_block));
        if (temp_pointer != nullptr)
        {
            get_prev_available_block(temp_pointer) = get_next_available_block(target_block);
        }
        else
        {
            get_prev_available_block(get_next_available_block(target_block)) = target_block;
        }
    }
    occupy_block(target_block);
    get_block_allocator(target_block) = this;
    target_next = get_next_available_block(target_block);
    if (target_prev != nullptr)
    {
        get_next_available_block(target_prev) = target_next;
    }
    if (target_block == first_block)
    {
        first_block = target_next;
    }
    get_prev_available_block(target_next) = target_prev;

    get_allocator_available_size() -= 1 << target_size;


    debug_with_guard(get_typename() + "::allocate(size_t value_size, size_t values_count) allocated: " + std::to_string(1 << target_size) + " bytes.");

    debug_with_guard(get_typename() + "::allocate(size_t value_size, size_t values_count) was finished");
    return reinterpret_cast<unsigned char*>(target_block) + get_occupied_block_meta_size();
}


void allocator_buddies_system::deallocate(
        void *at)
{
    trace_with_guard(get_typename() + "::deallocate(void *at) was called");
    std::lock_guard<std::mutex> guard(get_mutex());

    if (at == nullptr)
    {
        return;
    }
    at = reinterpret_cast<unsigned char*>(at) - get_occupied_block_meta_size();
    auto* at_allocator = get_block_allocator(at);

    if (at_allocator != this || !belong_trusted_memory(at))
    {
        error_with_guard(get_typename() + "::deallocate(void *at) trying to deallocate non-related memory");
        throw std::logic_error(get_typename() + "::deallocate(void *at) trying to deallocate non-related memory");
    }

    block_pointer_t temp_pointer = at;
    block_pointer_t buddy = get_buddy(temp_pointer);
    block_pointer_t next_block = nullptr;
    block_pointer_t prev_block = nullptr;
    block_size_t curr_size = 0;
    block_size_t data_size = get_allocator_data_size();
    block_size_t exempted_size = 1 << get_block_data_size(temp_pointer);


    if (block_is_occupied(buddy))
    {
        avail_block(temp_pointer);
    }


    while (!block_is_occupied(buddy) && curr_size != data_size && curr_size == get_block_data_size(buddy))
    {
        avail_block(temp_pointer);
        next_block = get_next_available_block(buddy);
        prev_block = get_prev_available_block(buddy);

        get_next_available_block(temp_pointer) = next_block;
        if (next_block)
        {
            get_prev_available_block(next_block) = temp_pointer;
        }
        get_prev_available_block(temp_pointer) = prev_block;
        if (prev_block)
        {
            get_next_available_block(prev_block) = temp_pointer;
        }
        if (*reinterpret_cast<unsigned char*>(temp_pointer) & curr_size)
        {
            temp_pointer = get_buddy(temp_pointer);
        }
        curr_size = get_block_data_size(temp_pointer) + 1;
        set_block_size(temp_pointer) = curr_size;
    }
    information_with_guard(get_typename() + "::deallocate(void *) : available size is " +
                           std::to_string(get_allocator_available_size()) + " bytes.");
    get_allocator_available_size() += exempted_size;
    at = reinterpret_cast<unsigned char*>(at) + get_available_block_meta_size();
    std::ostringstream out_stream(exempted_size > 0 ? " with data:" : "", std::ios::ate);
    out_stream << std::hex << std::setfill('0');

    for (size_t i = 0; i < exempted_size; ++i)
    {
        out_stream << " 0x" << std::setw(2) << static_cast<unsigned int>(
                *reinterpret_cast<unsigned char*>(at) + i);
    }

    debug_with_guard(get_typename() + "::deallocate(void *) : deallocated " + std::to_string(exempted_size)
                     + "(+" + std::to_string(get_occupied_block_meta_size()) + ") bytes" + out_stream.str() + ".");
    log_blocks_info(get_typename() + "::deallocate(void *)");
    information_with_guard(get_typename() + "::deallocate(void *) : available size is " +
                           std::to_string(get_allocator_available_size()) + " bytes.")->
            trace_with_guard(get_typename() + "::deallocate(void *) : finished.")->
            debug_with_guard(get_typename() + "::deallocate(void *) : finished.")->
            trace_with_guard(get_typename() + "::deallocate(void *at) finished");
}

inline void allocator_buddies_system::set_fit_mode(
        allocator_with_fit_mode::fit_mode mode)
{
    auto* temp_pointer = reinterpret_cast<unsigned char*>(_trusted_memory);
    temp_pointer += sizeof(allocator*) + sizeof(logger*) + sizeof(std::mutex);
    *reinterpret_cast<allocator_with_fit_mode::fit_mode*>(temp_pointer) = mode;
}

inline allocator *allocator_buddies_system::get_allocator() const
{
    return *reinterpret_cast<allocator**>(reinterpret_cast<unsigned char*>(_trusted_memory));
}

std::vector<allocator_test_utils::block_info> allocator_buddies_system::get_blocks_info() const noexcept
{
    std::lock_guard<std::mutex> mutex (get_mutex());
    trace_with_guard(get_typename() + "::get_blocks_info() was called");
    std::vector<allocator_test_utils::block_info> blocks_info = create_blocks_info();
    trace_with_guard(get_typename() + "::get_blocks_info() finished");
    return blocks_info;
}

inline logger *allocator_buddies_system::get_logger() const
{
    return *reinterpret_cast<logger**>(reinterpret_cast<unsigned char*>(_trusted_memory) + sizeof(allocator*));
}

inline std::string allocator_buddies_system::get_typename() const noexcept
{
    return "allocator_buddies_system";
}
//block : unsigned char (power) + bool (flag) + pointer + pointer + allocator*

inline allocator::block_size_t allocator_buddies_system::get_available_block_meta_size() const
{
    return sizeof(unsigned char) + sizeof(block_pointer_t) * 2;
}

inline allocator::block_size_t allocator_buddies_system::get_occupied_block_meta_size() const
{
    return sizeof(unsigned char) + sizeof (allocator*);
}

//allocator: allocator* + logger* + mutex + fit_mode + unsigned char (power) + block_size_t + pointer

inline allocator::block_size_t allocator_buddies_system::get_allocator_meta_size() const
{
    return sizeof(allocator*) + sizeof(logger*) + sizeof(std::mutex) + sizeof(fit_mode) + sizeof(unsigned char) + sizeof(block_size_t) + sizeof(block_pointer_t);
}



std::vector<allocator_test_utils::block_info> allocator_buddies_system::create_blocks_info() const noexcept
{
    trace_with_guard(get_typename() + "::create_blocks_info() was called");
    std::vector<allocator_test_utils::block_info> blocks_info(0);
    block_pointer_t curr_block = reinterpret_cast<unsigned char*>(_trusted_memory) + get_allocator_meta_size();
    bool is_occupied;

    block_size_t block_size = 0;

    while (belong_trusted_memory(curr_block))
    {
        is_occupied = block_is_occupied(curr_block);
        block_size = 1 << get_block_data_size(curr_block);
        blocks_info.push_back({.block_size = block_size, .is_block_occupied = is_occupied});
        curr_block = reinterpret_cast<unsigned char*>(curr_block) + block_size;
    }
    trace_with_guard(get_typename() + "::create_blocks_info() finished");
    return blocks_info;

}

inline std::mutex &allocator_buddies_system::get_mutex() const
{
    auto* temp_pointer = reinterpret_cast<unsigned char*>(_trusted_memory);
    temp_pointer += sizeof(allocator*) + sizeof(logger*);
    return *reinterpret_cast<std::mutex*>(temp_pointer);
}

inline unsigned char allocator_buddies_system::get_allocator_data_size() const
{
    auto* temp_pointer = reinterpret_cast<unsigned char*>(_trusted_memory);
    temp_pointer += sizeof(allocator*) + sizeof(class logger*) + sizeof(std::mutex) + sizeof(fit_mode);
    return *reinterpret_cast<unsigned char*>(temp_pointer);
}

inline unsigned char allocator_buddies_system::get_power_of_size(allocator::block_size_t size) const
{
    return static_cast<unsigned char>(ceil(log2(size)));
}

inline allocator::block_size_t &allocator_buddies_system::get_allocator_available_size() const
{
    auto* temp_pointer = reinterpret_cast<unsigned char*>(_trusted_memory);
    temp_pointer += sizeof(allocator*) + sizeof(class logger*) + sizeof(std::mutex) + sizeof(fit_mode) + sizeof(unsigned char);
    return *reinterpret_cast<block_size_t*>(temp_pointer);
}

inline allocator::block_pointer_t &allocator_buddies_system::get_first_available_block() const
{
    auto* temp_pointer = reinterpret_cast<unsigned char*>(_trusted_memory);
    temp_pointer += sizeof(allocator*) + sizeof(logger*) + sizeof(std::mutex)
                    + sizeof(fit_mode) + sizeof(unsigned char) + sizeof(block_size_t);
    return *reinterpret_cast<block_pointer_t*>(temp_pointer);
}
//unsigned char (power)  pointer + pointer - free block
//unsigned char (power) allocator* - occupied block

inline allocator::block_pointer_t &allocator_buddies_system::get_next_available_block(allocator::block_pointer_t block) const
{
    return *reinterpret_cast<block_pointer_t*>(reinterpret_cast<unsigned char*>(block) + sizeof(unsigned char) + sizeof (block_pointer_t));
}

inline allocator::block_pointer_t &allocator_buddies_system::get_prev_available_block(allocator::block_pointer_t block) const
{
    return *reinterpret_cast<block_pointer_t*>(reinterpret_cast<unsigned char*>(block) + sizeof(unsigned char));
}

inline allocator *&allocator_buddies_system::get_block_allocator(allocator::block_pointer_t block) const
{
    return *reinterpret_cast<allocator**>(reinterpret_cast<unsigned char*>(block) + sizeof(unsigned char));
}

inline bool allocator_buddies_system::block_is_occupied(allocator::block_pointer_t block) const noexcept
{
    return static_cast<bool>(*reinterpret_cast<unsigned char*>(block) >> 7);
}

inline unsigned char allocator_buddies_system::get_block_data_size(allocator::block_pointer_t block) const
{
    return *reinterpret_cast<unsigned char*>(block) & 127;
}

inline void allocator_buddies_system::occupy_block(allocator::block_pointer_t block) const
{

    *reinterpret_cast<unsigned char*>(block) |= (1 << 7);
}

inline void allocator_buddies_system::avail_block(allocator::block_pointer_t block) const
{
    *reinterpret_cast<unsigned char*>(block) &= ~(1 << 7);
}

inline allocator_with_fit_mode::fit_mode &allocator_buddies_system::get_fit_mode() const
{
    auto* temp_pointer = reinterpret_cast<unsigned char*>(_trusted_memory);
    temp_pointer += sizeof(allocator*) + sizeof(logger*) + sizeof(std::mutex);
    return *reinterpret_cast<fit_mode*>(temp_pointer);
}

inline unsigned char &allocator_buddies_system::set_block_size(allocator::block_pointer_t block) const
{
    return *reinterpret_cast<unsigned char*>(block);
}

allocator::block_pointer_t allocator_buddies_system::get_buddy(allocator::block_pointer_t block) const
{
    auto temp_pointer = reinterpret_cast<unsigned char*>(block) - reinterpret_cast<unsigned char*>(_trusted_memory) - get_allocator_meta_size();
    auto* buddy = (temp_pointer ^ 1 << get_block_data_size(block)) + reinterpret_cast<unsigned char*>(_trusted_memory) + get_allocator_meta_size();
    return reinterpret_cast<block_pointer_t>(buddy);
}

inline bool allocator_buddies_system::belong_trusted_memory(allocator::block_pointer_t block) const
{
    auto allocator_meta_size = get_allocator_meta_size();
    auto* mem_begin = reinterpret_cast<unsigned char*>(_trusted_memory) + allocator_meta_size;
    auto* mem_end = reinterpret_cast<unsigned char*>(_trusted_memory) + allocator_meta_size + (1 << get_allocator_data_size());
    return (block >= mem_begin && block < mem_end);
}


void allocator_buddies_system::log_blocks_info(const std::string& func_name) const
{
    std::ostringstream out_string;
    auto blocks_info = create_blocks_info();
    auto meta_size = get_occupied_block_meta_size();
    for (auto data: blocks_info)
    {
        if (data.is_block_occupied)
        {
            out_string << "occupied " << data.block_size - meta_size << "|";
        }
        else
        {
            out_string << "available " << data.block_size << "|";
        }
    }
    debug_with_guard(func_name + " memory status: |" + out_string.str());
}