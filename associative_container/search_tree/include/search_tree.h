#ifndef MATH_PRACTICE_AND_OPERATING_SYSTEMS_SEARCH_TREE_H
#define MATH_PRACTICE_AND_OPERATING_SYSTEMS_SEARCH_TREE_H

#include <iostream>
#include <functional>
#include <stack>
#include <vector>

#include <typename_holder.h>
#include <allocator.h>
#include <allocator_guardant.h>
#include <associative_container.h>
#include <logger.h>
#include <logger_guardant.h>
#include <not_implemented.h>

template<
    typename tkey,
    typename tvalue>
class search_tree:
    public associative_container<tkey, tvalue>,
    protected typename_holder,
    protected allocator_guardant,
    protected logger_guardant
{

public:
    
    struct common_node
    {
    
    public:
        
        typename associative_container<tkey, tvalue>::key_value_pair *keys_and_values;
        
        common_node **subtrees;
        
        size_t virtual_size;
    
    public:
    
        explicit common_node(
            typename associative_container<tkey, tvalue>::key_value_pair *keys_and_values,
            common_node **subtrees,
            size_t t);
        
        virtual ~common_node() noexcept;
        
    };
    
    #pragma region target operations associated exception types
    
    class insertion_of_existent_key_attempt_exception_exception final:
        public std::logic_error
    {
    
    private:
        
        tkey _key;
    
    public:
        
        explicit insertion_of_existent_key_attempt_exception_exception(
            tkey const &key);
        
    public:
        
        tkey const &get_key() const noexcept;
    
    };
    
    class obtaining_of_nonexistent_key_attempt_exception final:
        public std::logic_error
    {
    
    private:
        
        tkey _key;
        
    public:
        
        explicit obtaining_of_nonexistent_key_attempt_exception(
            tkey const &key);
        
    public:
        
        tkey const &get_key() const noexcept;
        
    };
    
    class updating_of_nonexistent_key_attempt_exception final:
        public std::logic_error
    {
    
    private:
        
        tkey _key;
        
    public:
        
        explicit updating_of_nonexistent_key_attempt_exception(
            tkey const &key);
        
    public:
        
        tkey const &get_key() const noexcept;
        
    };
    
    class disposal_of_nonexistent_key_attempt_exception final:
        public std::logic_error
    {
    
    private:
        
        tkey _key;
    
    public:
        
        explicit disposal_of_nonexistent_key_attempt_exception(
            tkey const &key);
        
    public:
        
        tkey const &get_key() const noexcept;
    
    };
    
    #pragma endregion target operations associated exception types
    
protected:
    
    std::function<int(tkey const &, tkey const &)> _keys_comparer;
    logger *_logger;
    allocator *_allocator;
    void *_root; // todo make common_node
    
protected:

    #pragma region node operations

    common_node *create_node(
        size_t t) const;
    
    void destroy_node(
        common_node *to_destroy);
    
    int node_find_path(
        typename search_tree<tkey, tvalue>::common_node const *node,
        tkey const &key,
        size_t left_bound_inclusive,
        size_t right_bound_inclusive);
    
    void node_insert(
        typename search_tree<tkey, tvalue>::common_node *node,
        typename associative_container<tkey, tvalue>::key_value_pair &&kvp,
        size_t subtree_index,
        typename search_tree<tkey, tvalue>::common_node *right_subtree);
    
    std::pair<typename search_tree<tkey, tvalue>::common_node *, typename associative_container<tkey, tvalue>::key_value_pair> node_split(
        typename search_tree<tkey, tvalue>::common_node *node,
        typename associative_container<tkey, tvalue>::key_value_pair &&kvp,
        size_t subtree_index,
        typename search_tree<tkey, tvalue>::common_node *right_subtree);
    
    void node_merge(
        typename search_tree<tkey, tvalue>::common_node *parent,
        size_t left_subtree_index);
    
    #pragma endregion common node operations
    
protected:
    
    std::stack<std::pair<typename search_tree<tkey, tvalue>::common_node**, int>> find_path(
        tkey const &key);
    
protected:
    
    explicit search_tree(
        std::function<int(tkey const &, tkey const &)> keys_comparer = typename associative_container<tkey, tvalue>::default_key_comparer(),
        allocator *allocator = nullptr,
        logger *logger = nullptr);
    
public:
    
    virtual std::vector<typename associative_container<tkey, tvalue>::key_value_pair> obtain_between(
        tkey const &lower_bound,
        tkey const &upper_bound,
        bool lower_bound_inclusive,
        bool upper_bound_inclusive) = 0;

protected:
    
    [[nodiscard]] inline allocator *get_allocator() const final;
    
    [[nodiscard]] inline logger *get_logger() const final;

private:
    
    inline std::string get_typename() const noexcept override;
    
};

#pragma region common node implementation

template<
    typename tkey,
    typename tvalue>
search_tree<tkey, tvalue>::common_node::common_node(
    typename associative_container<tkey, tvalue>::key_value_pair *keys_and_values,
    common_node **subtrees,
    size_t t):
        keys_and_values(keys_and_values),
        subtrees(subtrees),
        virtual_size(0)
{
    for (size_t i = 0; i < 2*t; ++i)
    {
        subtrees[i] = nullptr;
    }
}

template<
    typename tkey,
    typename tvalue>
search_tree<tkey, tvalue>::common_node::~common_node() noexcept
{
    virtual_size = 0;
}

#pragma endregion common node implementation

#pragma region target operations associated exception types implementation

template<
    typename tkey,
    typename tvalue>
search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_exception_exception::insertion_of_existent_key_attempt_exception_exception(
    tkey const &key):
        std::logic_error("Attempt to insert already existing key inside the tree."),
        _key(key)
{ }

template<
    typename tkey,
    typename tvalue>
tkey const &search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_exception_exception::get_key() const noexcept
{
    return _key;
}

template<
    typename tkey,
    typename tvalue>
search_tree<tkey, tvalue>::obtaining_of_nonexistent_key_attempt_exception::obtaining_of_nonexistent_key_attempt_exception(
    tkey const &key):
        std::logic_error("Attempt to obtain a value by non-existing key from the tree."),
        _key(key)
{ }

template<
    typename tkey,
    typename tvalue>
tkey const &search_tree<tkey, tvalue>::obtaining_of_nonexistent_key_attempt_exception::get_key() const noexcept
{
    return _key;
}

template<
    typename tkey,
    typename tvalue>
search_tree<tkey, tvalue>::updating_of_nonexistent_key_attempt_exception::updating_of_nonexistent_key_attempt_exception(
    tkey const &key):
        std::logic_error("Attempt to update a value by non-existing key from the tree."),
        _key(key)
{ }

template<
    typename tkey,
    typename tvalue>
tkey const &search_tree<tkey, tvalue>::updating_of_nonexistent_key_attempt_exception::get_key() const noexcept
{
    return _key;
}

template<
    typename tkey,
    typename tvalue>
search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_exception::disposal_of_nonexistent_key_attempt_exception(
    tkey const &key):
        std::logic_error("Attempt to dispose a value by non-existing key from the tree."),
        _key(key)
{ }

template<
    typename tkey,
    typename tvalue>
tkey const &search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_exception::get_key() const noexcept
{
    return _key;
}

#pragma endregion target operations associated exception types implementation

#pragma region common node operations implementation

template<
    typename tkey,
    typename tvalue>
typename search_tree<tkey, tvalue>::common_node *search_tree<tkey, tvalue>::create_node(
    size_t t) const
{
    typename associative_container<tkey, tvalue>::key_value_pair *keys_and_values = nullptr;
    common_node **subtrees = nullptr;
    common_node *node = nullptr;
    
    try
    {
        keys_and_values = reinterpret_cast<typename associative_container<tkey, tvalue>::key_value_pair*>(
            allocate_with_guard(sizeof(typename associative_container<tkey, tvalue>::key_value_pair), 2 * t - 1));
            
        subtrees = reinterpret_cast<common_node**>(
            allocate_with_guard(sizeof(common_node*), 2 * t));
            
        node = reinterpret_cast<common_node*>(
            allocate_with_guard(sizeof(common_node), 1));
    }
    catch (std::bad_alloc const &)
    {
        deallocate_with_guard(keys_and_values);
        deallocate_with_guard(subtrees);
        deallocate_with_guard(node);
        throw;
    }
    
    allocator::construct(node, keys_and_values, subtrees, t);
    
    return node;
}
    
template<
    typename tkey,
    typename tvalue>
void search_tree<tkey, tvalue>::destroy_node(
    common_node *to_destroy)
{
    for (size_t i = 0; i < to_destroy->virtual_size; ++i)
    {
        allocator::destruct(to_destroy->keys_and_values + i);
    }
    
    deallocate_with_guard(to_destroy->keys_and_values);
    deallocate_with_guard(to_destroy->subtrees);
    allocator::destruct(to_destroy);
    deallocate_with_guard(to_destroy);
}

template<
    typename tkey,
    typename tvalue>
int search_tree<tkey, tvalue>::node_find_path(
    typename search_tree<tkey, tvalue>::common_node const *node,
    tkey const &key,
    size_t left_bound_inclusive,
    size_t right_bound_inclusive)
{
    int index;
    while (true)
    {
        index = (left_bound_inclusive + right_bound_inclusive) / 2;
        auto comparison_result = _keys_comparer(key, node->keys_and_values[index].key);
        
        if (comparison_result == 0)
        {
            return index;
        }
        
        if (left_bound_inclusive == right_bound_inclusive)
        {
            return -(index + (comparison_result < 0 ? 0 : 1) + 1);
        }
        
        if (comparison_result < 0)
        {
            right_bound_inclusive = index;
        }
        else
        {
            left_bound_inclusive = index + 1;
        }
    }
}

template<
    typename tkey,
    typename tvalue>
void search_tree<tkey, tvalue>::node_insert(
    typename search_tree<tkey, tvalue>::common_node *node,
    typename associative_container<tkey, tvalue>::key_value_pair &&kvp,
    size_t subtree_index,
    typename search_tree<tkey, tvalue>::common_node *right_subtree)
{
    allocator::construct(node->keys_and_values + node->virtual_size, std::move(kvp));
    node->subtrees[node->virtual_size + 1] = right_subtree;
    
    for (size_t i = 0; i < node->virtual_size - subtree_index; ++i)
    {
        std::swap(node->keys_and_values[node->virtual_size - i], node->keys_and_values[node->virtual_size - (i + 1)]);
        std::swap(node->subtrees[node->virtual_size + 1 - i], node->subtrees[node->virtual_size - i]);
    }
    
    ++node->virtual_size;
}

template<
    typename tkey,
    typename tvalue>
std::pair<typename search_tree<tkey, tvalue>::common_node *, typename associative_container<tkey, tvalue>::key_value_pair>
search_tree<tkey, tvalue>::node_split(
    typename search_tree<tkey, tvalue>::common_node *node,
    typename associative_container<tkey, tvalue>::key_value_pair &&kvp,
    size_t subtree_index,
    typename search_tree<tkey, tvalue>::common_node *right_subtree)
{
    size_t const t = (node->virtual_size + 1) / 2;
    size_t const mediant_index = t;
    common_node *new_node = create_node(t); // try create node before structure change
    
    if (subtree_index != mediant_index)
    {
        // mediant may shift right after "insertion" of kvp, so if subtree_index < mediant_index,
        // we take left neighbour of pre-insert mediant as post-insert mediant
        if (subtree_index < mediant_index)
        {
            std::swap(node->keys_and_values[mediant_index - 1], kvp);
            std::swap(node->subtrees[mediant_index], right_subtree);
            
            for (size_t i = mediant_index - 1; i > subtree_index; --i)
            {
                std::swap(node->keys_and_values[i - 1], node->keys_and_values[i]);
                std::swap(node->subtrees[i], node->subtrees[i + 1]);
            }
        }
        else
        {
            std::swap(node->keys_and_values[mediant_index], kvp);
            std::swap(node->subtrees[mediant_index + 1], right_subtree);
            
            for (size_t i = mediant_index + 1; i < subtree_index; ++i)
            {
                std::swap(node->keys_and_values[i], node->keys_and_values[i - 1]);
                std::swap(node->subtrees[i + 1], node->subtrees[i]);
            }
        }
    }
    
    for (size_t i = 0; i < t - 1; ++i)
    {
        allocator::construct(new_node->keys_and_values + i, std::move(node->keys_and_values[t + i]));
        allocator::destruct(node->keys_and_values + t + i);
        
        std::swap(new_node->subtrees[1 + i], node->subtrees[t + 1 + i]);
    }
    
    new_node->subtrees[0] = right_subtree;
    
    new_node->virtual_size = t - 1;
    node->virtual_size = t;
    
    return std::make_pair(new_node, std::move(kvp));
}

template<
    typename tkey,
    typename tvalue>
void search_tree<tkey, tvalue>::node_merge(
    typename search_tree<tkey, tvalue>::common_node *parent,
    size_t parent_index)
{
    if (parent_index >= parent->virtual_size)
    {
        throw std::logic_error("invalid btree merge");
    }
    
    auto *left_subtree = parent->subtrees[parent_index];
    auto *right_subtree = parent->subtrees[parent_index + 1];
    
    allocator::construct(left_subtree->keys_and_values + left_subtree->virtual_size, std::move(parent->keys_and_values[parent_index]));
    ++left_subtree->virtual_size;
    
    for (size_t i = parent_index; i < parent->virtual_size; ++i)
    {
        std::swap(parent->keys_and_values[i], parent->keys_and_values[i+1]);
        std::swap(parent->subtrees[i+1], parent->subtrees[i+2]);
    }
    
    --parent->virtual_size;
    allocator::destruct(parent->keys_and_values + parent->virtual_size);
    
    for (size_t i = 0; i < right_subtree->virtual_size; ++i)
    {
        allocator::construct(left_subtree->keys_and_values + left_subtree->virtual_size, std::move(right_subtree->keys_and_values[i]));
        left_subtree->subtrees[left_subtree->virtual_size++] = right_subtree->subtrees[i];
    }
    left_subtree->subtrees[left_subtree->virtual_size] = right_subtree->subtrees[right_subtree->virtual_size];
    
    destroy_node(right_subtree);
}

#pragma endregion common node operations implementation

template<
    typename tkey,
    typename tvalue>
std::stack<std::pair<typename search_tree<tkey, tvalue>::common_node **, int>> search_tree<tkey, tvalue>::find_path(
    tkey const &key)
{
    std::stack<std::pair<common_node**, int>> result;
    
    int index = -1;
    if (_root == nullptr)
    {
        result.push(std::make_pair(reinterpret_cast<common_node**>(&_root), index));
        return result;
    }
    
    common_node **iterator = reinterpret_cast<common_node**>(&_root);
    while (*iterator != nullptr && index < 0)
    {
        index = node_find_path(*iterator, key, 0, (*iterator)->virtual_size - 1);
        result.push(std::make_pair(iterator, index));
        
        if (index < 0)
        {
            iterator = (*iterator)->subtrees - index - 1;
        }
    }
    
    return result;
}

template<
    typename tkey,
    typename tvalue>
search_tree<tkey, tvalue>::search_tree(
    std::function<int(tkey const &, tkey const &)> keys_comparer,
    allocator *allocator,
    logger *logger):
        _keys_comparer(keys_comparer),
        _allocator(allocator),
        _logger(logger),
        _root(nullptr)
{ }

template<
    typename tkey,
    typename tvalue>
[[nodiscard]] inline allocator *search_tree<tkey, tvalue>::get_allocator() const
{
    return _allocator;
}

template<
    typename tkey,
    typename tvalue>
[[nodiscard]] inline logger *search_tree<tkey, tvalue>::get_logger() const
{
    return _logger;
}

template<
    typename tkey,
    typename tvalue>
inline std::string search_tree<tkey, tvalue>::get_typename() const noexcept
{
    return "search_tree<tkey, tvalue>";
}

#endif //MATH_PRACTICE_AND_OPERATING_SYSTEMS_SEARCH_TREE_H