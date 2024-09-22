#include <not_implemented.h>

#include "../include/allocator_red_black_tree.h"

allocator_red_black_tree::~allocator_red_black_tree()
{
    if (get_logger() != nullptr)
    {
        trace_with_guard(get_typename() + "::~allocator_buddies_system() was called"); // TODO delete
    }
    get_mutex().~mutex();
    if (get_logger() != nullptr)
    {
        trace_with_guard(get_typename() + "::~allocator_buddies_system() finished"); // TODO delete
    }
    deallocate_with_guard(_trusted_memory);
}

allocator_red_black_tree::allocator_red_black_tree(
        size_t space_size,
        allocator *parent_allocator,
        logger *logger,
        allocator_with_fit_mode::fit_mode allocate_fit_mode)
{
    if (logger != nullptr)
    {
        logger->trace(get_typename() + "::allocator_buddies_system (size_t space_size, allocator *parent_allocator, logger *logger, allocator_with_fit_mode::fit_mode allocate_fit_mode) was called"); // TODO delete
    }
    if (space_size < get_available_block_meta_size())
    {
        if (logger != nullptr)
        {
            logger->error("Cannot initialize allocator: size of:" +  std::to_string(space_size) + "is too small. Minimum size is:" + std::to_string(get_occupied_block_meta_size()));
        }
        throw std::logic_error("Cannot initialize allocator");
    }

    block_size_t size_to_alloc = space_size + get_allocator_meta_size();

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

    *reinterpret_cast<block_size_t *>(temp_pointer) = space_size; //size
    temp_pointer += sizeof(block_size_t);

    *reinterpret_cast<block_size_t *>(temp_pointer) = space_size; //available
    temp_pointer += sizeof(block_size_t);


    block_pointer_t root_block = reinterpret_cast<unsigned char*>(_trusted_memory) + get_allocator_meta_size();
    *reinterpret_cast<block_pointer_t*>(temp_pointer) = root_block;
    temp_pointer += sizeof(block_pointer_t);

    *reinterpret_cast<block_pointer_t*>(temp_pointer) = nullptr; //prev_block;
    temp_pointer += sizeof(block_pointer_t);

    *reinterpret_cast<block_pointer_t*>(temp_pointer) = nullptr; //prev_block;
    temp_pointer += sizeof(block_pointer_t); // next_block

    *reinterpret_cast<block_size_t*>(temp_pointer) = space_size;
    temp_pointer += sizeof(block_size_t);

    *reinterpret_cast<char*>(temp_pointer) = '1';
    temp_pointer += sizeof(char);

    *reinterpret_cast<char*>(temp_pointer) = '0';
    temp_pointer += sizeof(char);

    *reinterpret_cast<block_pointer_t*>(temp_pointer) = nullptr; //parent;
    temp_pointer += sizeof(block_pointer_t);

    *reinterpret_cast<block_pointer_t*>(temp_pointer) = nullptr; //left;
    temp_pointer += sizeof(block_pointer_t); // next_block

    *reinterpret_cast<block_pointer_t*>(temp_pointer) = nullptr; //right;

    trace_with_guard(get_typename() + "::allocator_buddies_system (size_t space_size, allocator *parent_allocator, logger *logger, allocator_with_fit_mode::fit_mode allocate_fit_mode) finished"); // TODO delete
}

//allocator_red_black_tree::allocator_red_black_tree(
//    allocator_red_black_tree const &other)
//{
//    throw not_implemented("allocator_red_black_tree::allocator_red_black_tree(allocator_red_black_tree const &)", "your code should be here...");
//}
//
//allocator_red_black_tree &allocator_red_black_tree::operator=(
//    allocator_red_black_tree const &other)
//{
//    throw not_implemented("allocator_red_black_tree &allocator_red_black_tree::operator=(allocator_red_black_tree const &)", "your code should be here...");
//}

allocator_red_black_tree::allocator_red_black_tree(
        allocator_red_black_tree &&other) noexcept:
        _trusted_memory(other._trusted_memory)
{
    other._trusted_memory = nullptr;
}

allocator_red_black_tree &allocator_red_black_tree::operator=(
        allocator_red_black_tree &&other) noexcept
{
    if (this != &other)
    {
        get_mutex().~mutex();
        deallocate_with_guard(_trusted_memory);

        _trusted_memory = other._trusted_memory;
        other._trusted_memory = nullptr;
    }
    return *this;
}

allocator::block_pointer_t allocator_red_black_tree::get_first_fit(block_size_t size)
{
//    trace_with_guard("first fit");
    block_pointer_t curr_block = get_root_block();

    block_size_t curr_block_size = get_block_data_size(curr_block);
    while (curr_block && curr_block_size <= size)
    {
        curr_block = get_right_child(curr_block);
        if (curr_block)
        {
            curr_block_size = get_block_data_size(curr_block);
        }
    }
    return curr_block;
}

allocator::block_pointer_t allocator_red_black_tree::get_worst_fit(block_size_t size)
{
//    trace_with_guard("worst fit");
    block_pointer_t curr_block = get_root_block();

    while (get_right_child(curr_block))
    {
        curr_block = get_right_child(curr_block);
    }
    return get_block_data_size(curr_block) >= size ? curr_block : nullptr;
}

allocator::block_pointer_t allocator_red_black_tree::get_best_fit(block_size_t size) //tODO
{
//    trace_with_guard("best fit");
    block_pointer_t curr_block = get_root_block();

    block_size_t curr_block_size = get_block_data_size(curr_block);

    while (curr_block_size < size)
    {
        curr_block = get_right_child(curr_block);
        if (curr_block)
        {
            curr_block_size = get_block_data_size(curr_block);
        }
        else
        {
            break;
        }
    }
    if (curr_block == nullptr)
    {
        return nullptr;
    }
    while (get_left_child(curr_block)) //TODO think
    {
        block_pointer_t left_child = get_left_child(curr_block);
        block_size_t left_child_size = get_block_data_size(left_child);
        if (left_child_size >= size)
        {
            curr_block = left_child;
        }
        else
        {
            break;
        }
    }
    return curr_block;
}




[[nodiscard]] void *allocator_red_black_tree::allocate(
        size_t value_size,
        size_t values_count)
{
    debug_with_guard("go!");
    std::lock_guard<std::mutex> guard(get_mutex());

    block_size_t requested_size = value_size * values_count;
    block_size_t size_to_alloc = requested_size + get_occupied_block_meta_size();
    if (size_to_alloc < get_available_block_meta_size())
    {
        size_to_alloc = get_available_block_meta_size();
        warning_with_guard("Requested size was changed");
    }
    allocator_with_fit_mode::fit_mode fit_mode = get_fit_mode();
    block_pointer_t target_block = nullptr;

    switch (fit_mode)
    {
        case allocator_with_fit_mode::fit_mode::the_worst_fit:
            target_block = get_worst_fit(size_to_alloc);
            break;
        case allocator_with_fit_mode::fit_mode::first_fit:
            target_block = get_first_fit(size_to_alloc);
            break;
        case allocator_with_fit_mode::fit_mode::the_best_fit:
            target_block = get_best_fit(size_to_alloc);
            break;
    }
    if (target_block == nullptr)
    {
        error_with_guard("Cannot allocate memory");
        throw std::logic_error("Cannot allocate memory");
    }
    block_size_t real_size = get_block_data_size(target_block);
    block_size_t difference = real_size - size_to_alloc;

    block_pointer_t  prev_to_target = get_prev_block(target_block);
    block_pointer_t  next_to_target = get_next_block(target_block);

    if (difference && difference < get_available_block_meta_size())
    {
        warning_with_guard("Size changed");
        size_to_alloc = real_size;
    }


    delete_from_tree(target_block, get_block_data_size(target_block));

    if (size_to_alloc < real_size)
    {
        block_pointer_t new_block = reinterpret_cast<block_pointer_t*>(reinterpret_cast<unsigned char*>(target_block) + size_to_alloc);
        block_size_t new_block_size = real_size - size_to_alloc;
        get_prev_block(new_block) = target_block;
        get_next_block(new_block) = get_next_block(target_block);
        if (get_next_block(target_block))
        {
            get_prev_block(get_next_block(target_block)) = new_block;
        }
        get_next_block(target_block) = new_block;
        *reinterpret_cast<block_size_t*>(reinterpret_cast<unsigned char*>(new_block) + 2 * sizeof(block_pointer_t)) = new_block_size;
        *reinterpret_cast<block_size_t*>(reinterpret_cast<unsigned char*>(target_block) + 2 * sizeof(block_pointer_t)) = size_to_alloc - get_occupied_block_meta_size();
        insert_to_tree(new_block, new_block_size);
    }
    occupy_block(target_block);
    get_block_allocator(target_block) = this;
    return reinterpret_cast<unsigned char*>(target_block) + get_occupied_block_meta_size();

}

void allocator_red_black_tree::insert_to_tree(allocator::block_pointer_t block, size_t size)
{

//    trace_with_guard("Insertion started");
    block_pointer_t parent = nullptr;
    block_pointer_t current = get_root_block();

    set_red(block);
    get_left_child(block) = nullptr;
    get_right_child(block) = nullptr;
    get_parent(block) = nullptr;

    if (current == nullptr)
    {
        set_black(block);
        get_root_block() = block;
        get_parent(get_root_block()) = nullptr;
        return;
    }

    while (true)
    {
        parent = current;
        current = size <= get_block_data_size(current) ? get_left_child(parent) : get_right_child(parent);
        if (current == nullptr)
        {
            break;
        }
    }
    set_red(block);
    get_block_data_size(block) = size;
//    *reinterpret_cast<block_size_t*>(reinterpret_cast<unsigned char*>(block) + 2 * sizeof(block_pointer_t)) = size;
    avail_block(block);
    get_parent(block) = parent;
    get_left_child(block) = nullptr;
    get_right_child(block) = nullptr;

    if (size < get_block_data_size(parent))
    {
        get_left_child(parent) = block;
    }
    else
    {
        get_right_child(parent) = block;
    }
    balance_after_insert(block);
}

void allocator_red_black_tree::delete_from_tree(allocator::block_pointer_t block, size_t size)
{
    block_pointer_t parent = nullptr;
    block_pointer_t current = get_root_block();

    while (true)
    {
        if (current == block)
        {
            break;
        }
        current = size <= get_block_data_size(current) ? get_left_child(current) : get_right_child(current);

    }

    if (get_left_child(current) && get_right_child(current))
    {

        block_pointer_t node_to_dispose = current;
        block_pointer_t curr_node = get_left_child(current);

        while (get_right_child(curr_node))
        {
            curr_node = get_right_child(curr_node);
        }
        current = curr_node;

        if (block_is_black(current) ^ block_is_black(node_to_dispose))
        {
            if (block_is_black(current))
            {
                set_red(current);
                set_black(node_to_dispose);
            }
            else
            {
                set_red(node_to_dispose);
                set_black(current);
            }
        }


        block_pointer_t &ptr_to_current = get_left_child(get_parent(current)) == current
                                          ? get_left_child(get_parent(current))
                                          : get_right_child(get_parent(current));

        bool is_neighbour = get_parent(current) == node_to_dispose;

        if (is_neighbour)
        {

            if (get_parent(node_to_dispose))
            {
                block_pointer_t &ptr_to_dispose = get_left_child(get_parent(node_to_dispose)) == node_to_dispose
                                                  ? get_left_child(get_parent(node_to_dispose))
                                                  : get_right_child(get_parent(node_to_dispose));

                ptr_to_dispose = current;
            }
            else
            {
                get_root_block() = current;
            }

            if (current == get_right_child(get_parent(current)))
            {

                if (get_left_child(node_to_dispose))
                {
                    get_parent(get_left_child(node_to_dispose)) = current;
                }
                if (get_left_child(current))
                {
                    get_parent(get_left_child(current)) = node_to_dispose;
                }
                std::swap(get_left_child(node_to_dispose), get_left_child(current));

                get_parent(current) = get_parent(node_to_dispose);

                if (get_right_child(current))
                {
                    get_parent(get_right_child(current)) = node_to_dispose;
                }

                get_right_child(node_to_dispose) = get_right_child(current);

                get_parent(node_to_dispose) = current;
                get_right_child(current) = node_to_dispose;
            }
            else
            {
                if (get_right_child(node_to_dispose))
                {
                    get_parent(get_right_child(node_to_dispose)) = current;
                }
                if (get_right_child(current))
                {
                    get_parent(get_right_child(current)) = node_to_dispose;
                }
                std::swap(get_right_child(node_to_dispose), get_right_child(current));

                get_parent(current) = get_parent(node_to_dispose);

                if (get_left_child(current))
                {
                    get_parent(get_left_child(current)) = node_to_dispose;
                }

                get_left_child(node_to_dispose) = get_left_child(current);

                get_parent(node_to_dispose) = current;
                get_left_child(current) = node_to_dispose;
            }

        }
        else
        {
            if (get_parent(node_to_dispose))
            {
                block_pointer_t &ptr_to_dispose = get_left_child(get_parent(node_to_dispose)) == node_to_dispose
                                                  ? get_left_child(get_parent(node_to_dispose))
                                                  : get_right_child(get_parent(node_to_dispose));

                std::swap(ptr_to_dispose, ptr_to_current);
            }
            else
            {
                ptr_to_current = node_to_dispose;
                get_root_block() = current;
                get_parent(get_root_block()) = nullptr;
            }

            // left child swap
            if (get_left_child(current))
            {
                std::swap(get_parent(get_left_child(current)), get_parent(get_left_child(node_to_dispose)));
            }
            else
            {
                get_parent(get_left_child(node_to_dispose)) = current;
            }

            // right child swap
            if (get_right_child(node_to_dispose))
            {
                get_parent(get_right_child(node_to_dispose)) = current;
            }

            std::swap(get_parent(current), get_parent(node_to_dispose));
            std::swap(get_left_child(current), get_left_child(node_to_dispose));
            std::swap(get_right_child(current), get_right_child(node_to_dispose));
        }

        std::swap(node_to_dispose, current);
    }


    block_pointer_t tmp = get_left_child(current) == nullptr
                          ? get_right_child(current)
                          : get_left_child(current);


    if (get_parent(current))
    {

        block_pointer_t &ptr_to_current = get_left_child(get_parent(current)) == current
                                          ? get_left_child(get_parent(current))
                                          : get_right_child(get_parent(current));
        ptr_to_current = tmp;

    }
    else
    {
        get_root_block() = tmp;
        if (tmp)
        {
            get_parent(get_root_block()) = nullptr;
        }
    }

    balance_after_delete(tmp, get_parent(current), current);

}



void allocator_red_black_tree::balance_after_insert(allocator::block_pointer_t new_block)
{
    if (new_block == nullptr)
    {
        return;
    }

    block_pointer_t curr_block = new_block;
    block_pointer_t parent = get_parent(curr_block);
    if (!parent)
    {
        set_black(curr_block);
        return;
    }

    if (block_is_black(curr_block) || block_is_black(parent))
    {
        return;
    }

    block_pointer_t uncle = get_uncle(curr_block);
    block_pointer_t granddad = get_parent(parent);

    if (get_left_child(granddad) == parent)
    {
        if (!block_is_black(uncle))
        {
            set_black(parent);
            set_black(uncle);
            set_red(granddad);
            balance_after_insert(granddad);
        }
        else
        {
            set_black(parent);
            set_red(granddad);
            if (get_right_child(parent) == curr_block)
            {
                set_red(parent);
                set_black(curr_block);
                left_rotate(parent);
            }
            right_rotate(granddad);
        }
    }
    else
    {
        if (!block_is_black(uncle))
        {
            set_black(parent);
            set_black(uncle);
            set_red(granddad);
            balance_after_insert(granddad);
        }
        else
        {
            set_black(curr_block);
            set_red(granddad);
            if (get_left_child(parent) == curr_block)
            {
                right_rotate(parent);
            }
            left_rotate(granddad);
        }
    }
}



void  allocator_red_black_tree::balance_after_delete(block_pointer_t block, block_pointer_t parent, block_pointer_t old_block)
{

    if (block == nullptr && parent == nullptr)
    {
        return;
    }

    if (!block_is_black(old_block))
    {
        return;
    }

    if (!block_is_black(block))
    {
        set_black(block);
        return;
    }

    balance_black_leaf(block, parent);
}


void allocator_red_black_tree::balance_black_leaf(block_pointer_t block, block_pointer_t parent)

{
    if (parent == nullptr)
    {
        set_black(block);
        return;
    }

    if (get_left_child(parent) == block)
    {
        block_pointer_t brother = get_right_child(parent);
        if (!block_is_black(brother))
        {
            set_red(parent);
            set_black(brother);
            left_rotate(parent);
            balance_black_leaf(block, parent);
        }
        else
        {
            if(!block_is_black(get_right_child(brother)))
            {
                if (block_is_black(parent))
                {
                    set_black(brother);
                }
                else
                {
                    set_red(brother);
                }
                set_black(parent);
                set_black(get_right_child(brother));
                left_rotate(parent);
                return;
            }

            if(!block_is_black(get_left_child(brother)) && block_is_black(get_right_child(brother)))
            {
                if (block_is_black(parent))
                {
                    set_black(get_left_child(brother));
                }
                else
                {
                    set_red(get_left_child(brother));
                }
                set_black(parent); //parent
                right_rotate(get_right_child(parent));
                left_rotate(parent);
                return;
            }

            if (block_is_black(get_right_child(brother)) && block_is_black(get_left_child(brother)))
            {
                set_red(brother);
                if (!block_is_black(parent))
                {
                    set_black(parent);
                    return;
                }
                balance_black_leaf(parent, get_parent(parent));
            }
        }

    }
    else
    {
        block_pointer_t brother = get_left_child(parent);
        if (!block_is_black(brother))
        {
            set_red(parent);
            set_black(brother);
            right_rotate(parent);
            balance_black_leaf(block, parent);
        }
        else
        {
            if (!block_is_black(get_left_child(brother)))
            {
                if (block_is_black(parent))
                {
                    set_black(brother);
                }
                else
                {
                    set_red(brother);
                }
                set_black(parent);
                set_black(get_left_child(brother));
                right_rotate(parent);//
                return;
            }

            if (!block_is_black(get_right_child(brother)) && block_is_black(get_left_child(brother)))
            {
                if (block_is_black(parent))
                {
                    set_black(get_right_child(brother));
                }
                else
                {
                    set_red(get_right_child(brother));
                }
                set_black(parent);
                left_rotate(get_left_child(parent));
                right_rotate(parent);
                return;
            }

            if (block_is_black(get_left_child(brother)) && block_is_black(get_right_child(brother)))
            {
                set_red(brother);
                if (!block_is_black(parent))
                {
                    set_black(parent);
                    return;
                }
                balance_black_leaf(parent, parent);
            }
        }
    }
}


void allocator_red_black_tree::left_rotate(allocator::block_pointer_t block)
{
    block_pointer_t parent = get_parent(block);
    block_pointer_t new_root = get_right_child(block);
    block_pointer_t  root_left_child = get_left_child(new_root);
    get_right_child(block) = root_left_child;

    if (root_left_child)
    {
        get_parent(root_left_child) = block;
    }

    get_left_child(new_root) = block;
    get_parent(block) = new_root;

    get_parent(new_root) = parent;

    if (!parent)
    {
        get_root_block() = new_root;
        get_parent(get_root_block()) = nullptr;
    }
    else
    {
        if (block == get_right_child(parent))
        {
            get_right_child(parent) = new_root;
        }
        else
        {
            get_left_child(parent) = new_root;
        }
    }

}

void allocator_red_black_tree::right_rotate(allocator::block_pointer_t block)
{
    block_pointer_t parent = get_parent(block);
    block_pointer_t new_root = get_left_child(block);
    block_pointer_t  right_root_child = get_right_child(new_root);
    get_left_child(block) = right_root_child;
    if (right_root_child)
    {
        get_parent(right_root_child) = block;
    }

    get_right_child(new_root) = block;
    get_parent(block) = new_root;

    get_parent(new_root) = parent;

    if (!parent)
    {
        get_root_block() = new_root;
        get_parent(get_root_block()) = nullptr;
    }

    else
    {
        if (block == get_right_child(parent))
        {
            get_right_child(parent) = new_root;
        }
        else
        {
            get_left_child(parent) = new_root;
        }
    }
}


void allocator_red_black_tree::deallocate(
        void *at)
{
    std::lock_guard<std::mutex> guard(get_mutex());
    auto* at_allocator = get_block_allocator(reinterpret_cast<unsigned char*>(at) - get_occupied_block_meta_size());
    block_size_t allocator_meta_size = get_allocator_meta_size();
    auto* mem_begin = reinterpret_cast<unsigned char*>(_trusted_memory) + allocator_meta_size;
    auto* mem_end = reinterpret_cast<unsigned char*>(_trusted_memory) + allocator_meta_size + get_allocator_data_size();

    if (at < mem_begin || at >= mem_end)
    {
        error_with_guard(get_typename() + "::deallocate(void *at) trying to deallocate non-related memory");
        throw std::logic_error(get_typename() + "::deallocate(void *at) trying to deallocate non-related memory");
    }
    block_pointer_t block = reinterpret_cast<unsigned char*>(at) - get_occupied_block_meta_size();
    block_pointer_t &next_block = get_next_block(block);
    block_pointer_t &prev_block = get_prev_block(block);

    if (next_block && !block_is_occupied(next_block))
    {
        delete_from_tree(next_block, get_block_data_size(next_block));
        get_block_data_size(block) += get_block_data_size(next_block);

        if (get_next_block(next_block))
        {
            get_prev_block(get_next_block(next_block)) = block;
        }
        next_block = get_next_block(next_block);
    }

    if (prev_block && !block_is_occupied(prev_block))
    {
        delete_from_tree(prev_block, get_block_data_size(prev_block));
        get_block_data_size(prev_block) += get_block_data_size(block);

        if (next_block)
        {
            get_prev_block(next_block) = prev_block;
        }
        get_next_block(prev_block) = next_block;
        block = prev_block;
    }

//    get_allocator_data_size() -= get_block_data_size(block);

    //get_block_data_size(block) += get_available_block_meta_size(); // DONT FORGET ME TODO
    insert_to_tree(block, get_block_data_size(block));
}

inline void allocator_red_black_tree::set_fit_mode(
        allocator_with_fit_mode::fit_mode mode)
{
    auto* temp_pointer = reinterpret_cast<unsigned char*>(_trusted_memory);
    temp_pointer += sizeof(allocator*) + sizeof(logger*) + sizeof(std::mutex);
    *reinterpret_cast<allocator_with_fit_mode::fit_mode*>(temp_pointer) = mode;
}

inline allocator *allocator_red_black_tree::get_allocator() const
{
    return *reinterpret_cast<allocator**>(reinterpret_cast<unsigned char*>(_trusted_memory));
}

inline logger *allocator_red_black_tree::get_logger() const
{
    return *reinterpret_cast<logger**>(reinterpret_cast<unsigned char*>(_trusted_memory) + sizeof(allocator*));
}

inline std::mutex &allocator_red_black_tree::get_mutex() const
{
    return *reinterpret_cast<std::mutex*>(reinterpret_cast<unsigned char*>(_trusted_memory) + sizeof(allocator*) + sizeof(logger*));
}

inline allocator_with_fit_mode::fit_mode &allocator_red_black_tree::get_fit_mode() const
{
    return *reinterpret_cast<fit_mode*>(reinterpret_cast<unsigned char*>(_trusted_memory) + sizeof(allocator*) + sizeof(logger*) + sizeof(std::mutex));
}


std::vector<allocator_test_utils::block_info> allocator_red_black_tree::get_blocks_info() const noexcept
{
    throw not_implemented("std::vector<allocator_test_utils::block_info> allocator_red_black_tree::get_blocks_info() const noexcept", "your code should be here...");
}

inline std::string allocator_red_black_tree::get_typename() const noexcept
{
    return "allocator_red_black_tree";
}


allocator::block_size_t allocator_red_black_tree::get_available_block_meta_size()
{
    return 5 * sizeof(block_pointer_t) + sizeof(block_size_t) + 2 * sizeof(bool);
}
allocator::block_size_t allocator_red_black_tree::get_occupied_block_meta_size()
{
    return 2 * sizeof(block_pointer_t) + sizeof(block_size_t) + sizeof(allocator*);
}


inline allocator::block_pointer_t &allocator_red_black_tree::get_root_block() const
{
    auto* temp_pointer = reinterpret_cast<unsigned char*>(_trusted_memory);
    temp_pointer += sizeof(allocator*) + sizeof(logger*) + sizeof(std::mutex) + sizeof(fit_mode) + 2 * sizeof(block_size_t);
    return *reinterpret_cast<block_pointer_t*>(temp_pointer);
}


inline allocator::block_pointer_t &allocator_red_black_tree::get_prev_block(allocator::block_pointer_t block)
{
    return *reinterpret_cast<block_pointer_t*>(reinterpret_cast<unsigned char*>(block));
}

inline allocator::block_pointer_t &allocator_red_black_tree::get_next_block(allocator::block_pointer_t block)
{
    return *reinterpret_cast<block_pointer_t*>(reinterpret_cast<unsigned char*>(block) + sizeof(block_pointer_t));
}

allocator::block_size_t &allocator_red_black_tree::get_block_data_size(allocator::block_pointer_t block)
{
    return *reinterpret_cast<block_size_t*>(reinterpret_cast<unsigned char*>(block) + 2 * sizeof(block_pointer_t));
}


inline allocator::block_pointer_t &allocator_red_black_tree::get_parent(allocator::block_pointer_t block)
{
    return *reinterpret_cast<block_pointer_t*>(reinterpret_cast<unsigned char*>(block) + 2 * sizeof(block_pointer_t) + sizeof(size_t) + 2 * sizeof(bool));
}

inline allocator::block_pointer_t &allocator_red_black_tree::get_uncle(allocator::block_pointer_t block) const
{
    block_pointer_t parent = get_parent(block);
    if (parent == nullptr)
    {
        error_with_guard("Cannot allocate memory");
        throw std::logic_error("Parent does don exist");
    }
    block_pointer_t granddad = get_parent(parent);
    if (granddad == nullptr)
    {
        error_with_guard("Cannot allocate memory");
        throw std::logic_error("Granddad does don exist");
    }
    if (get_left_child(granddad) == parent)
    {
        return get_right_child(granddad);
    }
    return get_left_child(granddad);
}

inline allocator::block_pointer_t &allocator_red_black_tree::get_left_child(allocator::block_pointer_t block)
{
    return *reinterpret_cast<block_pointer_t*>(reinterpret_cast<unsigned char*>(block) + 2 * sizeof(block_pointer_t) + sizeof(size_t) + 2 * sizeof(bool) + sizeof(block_pointer_t));
}

inline allocator::block_pointer_t &allocator_red_black_tree::get_right_child(allocator::block_pointer_t block)
{
    return *reinterpret_cast<block_pointer_t*>(reinterpret_cast<unsigned char*>(block) + 2 * sizeof(block_pointer_t) + sizeof(size_t) + 2 * sizeof(bool) + 2 * sizeof(block_pointer_t));
}

inline allocator *&allocator_red_black_tree::get_block_allocator(allocator::block_pointer_t block)
{
    return *reinterpret_cast<allocator**>(reinterpret_cast<unsigned char*>(block) + 2 * (sizeof(block_pointer_t)+  sizeof(size_t) +  sizeof(bool)));
}

inline bool allocator_red_black_tree::block_is_occupied(allocator::block_pointer_t block) noexcept //black - 1; red - 0
{
    return *reinterpret_cast<char*>(reinterpret_cast<unsigned char*>(block) + 2 * sizeof(block_pointer_t) + sizeof(size_t) + sizeof(bool)) == '1';
}

inline void allocator_red_black_tree::occupy_block(allocator::block_pointer_t block)
{
    *reinterpret_cast<char*>(reinterpret_cast<unsigned char*>(block) + 2 * sizeof(block_pointer_t) + sizeof(size_t) + sizeof(bool)) = '1';
}

inline void allocator_red_black_tree::avail_block(allocator::block_pointer_t block)
{
    *reinterpret_cast<char*>(reinterpret_cast<unsigned char*>(block) + 2 * sizeof(block_pointer_t) + sizeof(size_t) + sizeof(bool)) = '0';
}

inline bool allocator_red_black_tree::block_is_black(allocator::block_pointer_t block) noexcept //black - 1; red - 0
{
    if (block == nullptr)
    {
        return true;
    }
    return *reinterpret_cast<char*>(reinterpret_cast<unsigned char*>(block) + 2 * sizeof(block_pointer_t) + sizeof(size_t)) == '1';

}

inline void allocator_red_black_tree::set_black(allocator::block_pointer_t block)
{
    *reinterpret_cast<char*>(reinterpret_cast<unsigned char*>(block) + 2 * sizeof(block_pointer_t) + sizeof(size_t)) = '1';
}

inline void allocator_red_black_tree::set_red(allocator::block_pointer_t block)
{
    *reinterpret_cast<char*>(reinterpret_cast<unsigned char*>(block) + 2 * sizeof(block_pointer_t) + sizeof(size_t)) = '0';
}

inline allocator::block_size_t allocator_red_black_tree::get_allocator_meta_size()
{
    return sizeof(allocator*) + sizeof(logger*) + sizeof(std::mutex) + sizeof(fit_mode) + 2 * sizeof(block_size_t) + sizeof(block_pointer_t);
}

inline allocator::block_size_t &allocator_red_black_tree::get_allocator_data_size() const
{
    auto* temp_pointer = reinterpret_cast<unsigned char*>(_trusted_memory);
    temp_pointer += sizeof(allocator*) + sizeof(class logger*) + sizeof(std::mutex) + sizeof(fit_mode);
    return *reinterpret_cast<block_size_t *>(temp_pointer);
}
