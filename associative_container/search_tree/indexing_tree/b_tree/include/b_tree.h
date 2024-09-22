#ifndef MATH_PRACTICE_AND_OPERATING_SYSTEMS_TEMPLATE_REPO_B_TREE_H
#define MATH_PRACTICE_AND_OPERATING_SYSTEMS_TEMPLATE_REPO_B_TREE_H

#include <search_tree.h>

#include <extra_utility.h>
#include <mutex>

template<
    typename tkey,
    typename tvalue>
class b_tree final : public search_tree<tkey, tvalue> {

public:
    
    #pragma region iterators definition
    
    class infix_iterator final
    {
        
        friend class b_tree<tkey, tvalue>;
        
    public:

        bool operator==(
            infix_iterator const &other) const noexcept;

        bool operator!=(
            infix_iterator const &other) const noexcept;

        infix_iterator &operator++();

        infix_iterator operator++(
            int not_used);
        
        std::tuple<size_t, size_t, tkey const &, tvalue &> operator*() const;
    
    private:
    
        infix_iterator(
            typename search_tree<tkey, tvalue>::common_node *node);
    
    private:
    
        std::stack<std::pair<typename search_tree<tkey, tvalue>::common_node*, int>> _state;
    
    };

    class infix_const_iterator final
    {
        
        friend class b_tree<tkey, tvalue>;
        
    public:

        bool operator==(
            infix_const_iterator const &other) const noexcept;

        bool operator!=(
            infix_const_iterator const &other) const noexcept;

        infix_const_iterator &operator++();

        infix_const_iterator operator++(
            int not_used);
        
        std::tuple<size_t, size_t, tkey const &, tvalue const &> operator*() const;

    private:
    
        infix_const_iterator(
            typename search_tree<tkey, tvalue>::common_node *node);
        
        infix_const_iterator(
            std::stack<std::pair<typename search_tree<tkey, tvalue>::common_node*, int>> path);
    
    private:
    
        std::stack<std::pair<typename search_tree<tkey, tvalue>::common_node*, int>> _state;
    
    };

    class infix_reverse_iterator final
    {
        friend class b_tree<tkey, tvalue>;

    public:

        bool operator==(
                infix_reverse_iterator const &other) const noexcept;

        bool operator!=(
                infix_reverse_iterator const &other) const noexcept;

        infix_reverse_iterator &operator++();

        infix_reverse_iterator operator++(
                int not_used);

        std::tuple<size_t, size_t, tkey const &, tvalue &> operator*() const;

    private:

        infix_reverse_iterator(
                typename search_tree<tkey, tvalue>::common_node *node);

    private:

        std::stack<std::pair<typename search_tree<tkey, tvalue>::common_node*, int>> _state;

    };

    class infix_const_reverse_iterator final
    {
        friend class b_tree<tkey, tvalue>;

    public:

        bool operator==(
                infix_const_reverse_iterator const &other) const noexcept;

        bool operator!=(
                infix_const_reverse_iterator const &other) const noexcept;

        infix_const_reverse_iterator &operator++();

        infix_const_reverse_iterator operator++(
                int not_used);

        std::tuple<size_t, size_t, tkey const &, tvalue const &> operator*() const;

    private:

        infix_const_reverse_iterator(
                typename search_tree<tkey, tvalue>::common_node *node);

        infix_const_reverse_iterator(
                std::stack<std::pair<typename search_tree<tkey, tvalue>::common_node*, int>> path);

    private:

        std::stack<std::pair<typename search_tree<tkey, tvalue>::common_node*, int>> _state;

    };


#pragma endregion iterators definition

public:
    
    #pragma region CRUD operations
    
    void insert(
        tkey const &key,
        tvalue const &value) override;

    void insert(
        tkey const &key,
        tvalue &&value) override;
    
    void update(
        tkey const &key,
        tvalue const &value) override;

    void update(
        tkey const &key,
        tvalue &&value) override;
    
    tvalue &obtain(
        tkey const &key) override;

    tvalue dispose(
        tkey const &key) override;

    std::vector<typename associative_container<tkey, tvalue>::key_value_pair> obtain_between(
        tkey const &lower_bound,
        tkey const &upper_bound,
        bool lower_bound_inclusive,
        bool upper_bound_inclusive) override;
    
    #pragma endregion CRUD operations
    
public:
    
    #pragma region BTree constructors, assignments, destructor
    
    explicit b_tree(
        size_t t,
        std::function<int(tkey const &, tkey const &)> keys_comparer = typename associative_container<tkey, tvalue>::default_key_comparer(),
        allocator *allocator = nullptr,
        logger *logger = nullptr);

    b_tree(
        b_tree<tkey, tvalue> const &other);

    b_tree<tkey, tvalue> &operator=(
        b_tree<tkey, tvalue> const &other);

    b_tree(
        b_tree<tkey, tvalue> &&other) noexcept;

    b_tree<tkey, tvalue> &operator=(
        b_tree<tkey, tvalue> &&other) noexcept;

    ~b_tree() = default;
    
    #pragma endregion BTree constructors, assignments, destructor
    
public:
    
    #pragma region iterators requesting
    
    infix_iterator begin_infix() const noexcept;

    infix_iterator end_infix() const noexcept;

    infix_const_iterator cbegin_infix() const noexcept;

    infix_const_iterator cend_infix() const noexcept;

    infix_reverse_iterator rbegin_infix() const noexcept;

    infix_reverse_iterator rend_infix() const noexcept;

    infix_const_reverse_iterator crbegin_infix() const noexcept;

    infix_const_reverse_iterator crend_infix() const noexcept;
    
    #pragma endregion iterators requesting
    
private:

    size_t _t;
    
    mutable std::mutex _mutex;
    
private:

    void insert_inner(
        typename associative_container<tkey, tvalue>::key_value_pair &&kvp,
        bool is_update);
public:
    size_t get_t() const
    {
        return _t;
    }

private:

    #pragma region utility functions

    inline size_t get_min_keys_count() const noexcept;
    
    inline size_t get_max_keys_count() const noexcept;
    
    typename search_tree<tkey, tvalue>::common_node *copy(
        typename search_tree<tkey, tvalue>::common_node const *node);
    
    void clear(
        typename search_tree<tkey, tvalue>::common_node *node);
    
    #pragma endregion utility functions

private:
    
    inline std::string get_typename() const noexcept override;
    
};

#pragma region infix iterator implementation

template<
    typename tkey,
    typename tvalue>
bool b_tree<tkey, tvalue>::infix_iterator::operator==(
    typename b_tree::infix_iterator const &other) const noexcept
{
    if (_state.empty() && other._state.empty())
    {
        return true;
    }
    
    if (_state.empty() ^ other._state.empty())
    {
        return false;
    }
    
    return _state.top() == other._state.top();
}

template<
    typename tkey,
    typename tvalue>
bool b_tree<tkey, tvalue>::infix_iterator::operator!=(
    typename b_tree::infix_iterator const &other) const noexcept
{
    return !(*this == other);
}

template<
    typename tkey,
    typename tvalue>
typename b_tree<tkey, tvalue>::infix_iterator &b_tree<tkey, tvalue>::infix_iterator::operator++()
{
    if (_state.empty())
    {
        return *this; // UB
    }
    
    auto *cur_node = _state.top().first;
    auto cur_pos = _state.top().second;
    
    if (cur_node->subtrees[0] != nullptr)
    {
        cur_node = cur_node->subtrees[cur_pos + 1];
        
        while (cur_node->subtrees[0] != nullptr)
        {
            _state.emplace(cur_node, -1);
            cur_node = cur_node->subtrees[0];
        }
        
        _state.emplace(cur_node, 0);
        
        return *this;
    }
    
    if (cur_pos + 1 < cur_node->virtual_size)
    {
        ++_state.top().second;
        return *this;
    }
    
    while (cur_pos + 1 == cur_node->virtual_size)
    {
        _state.pop();
        
        if (_state.empty())
        {
            return *this;
        }
        
        cur_node = _state.top().first;
        cur_pos = _state.top().second;
    }
    
    ++_state.top().second;
    
    return *this;
}

template<
    typename tkey,
    typename tvalue>
typename b_tree<tkey, tvalue>::infix_iterator b_tree<tkey, tvalue>::infix_iterator::operator++(
    int not_used)
{
    infix_iterator iter = *this;
    *this++;
    return iter;
}

template<
    typename tkey,
    typename tvalue>
std::tuple<size_t, size_t, tkey const &, tvalue &> b_tree<tkey, tvalue>::infix_iterator::operator*() const
{
    if (_state.empty())
    {
        throw std::logic_error("attempt to dereference invalid or end iterator");
    }
    
    auto *kvp = _state.top().first->keys_and_values;
    auto pos = _state.top().second;
    
    return std::tuple<size_t, size_t, tkey const &, tvalue &>(_state.size() - 1, pos, kvp[pos].key, kvp[pos].value);
}

template<
    typename tkey,
    typename tvalue>
b_tree<tkey, tvalue>::infix_iterator::infix_iterator(
    typename search_tree<tkey, tvalue>::common_node *node)
{
    if (node == nullptr)
    {
        return;
    }
    
    auto *iter = node;
    
    while (iter->subtrees[0] != nullptr)
    {
        _state.emplace(iter, -1);
        iter = iter->subtrees[0];
    }
    
    _state.emplace(iter, 0);
}

template<
    typename tkey,
    typename tvalue>
bool b_tree<tkey, tvalue>::infix_const_iterator::operator==(
    b_tree::infix_const_iterator const &other) const noexcept
{
    if (_state.empty() && other._state.empty())
    {
        return true;
    }
    
    if (_state.empty() ^ other._state.empty())
    {
        return false;
    }
    
    return _state.top() == other._state.top();
}

template<
    typename tkey,
    typename tvalue>
bool b_tree<tkey, tvalue>::infix_const_iterator::operator!=(
    b_tree::infix_const_iterator const &other) const noexcept
{
    return !(*this == other);
}

template<
    typename tkey,
    typename tvalue>
typename b_tree<tkey, tvalue>::infix_const_iterator &b_tree<tkey, tvalue>::infix_const_iterator::operator++()
{
    if (_state.empty())
    {
        return *this; // UB
    }
    
    auto *cur_node = _state.top().first;
    auto cur_pos = _state.top().second;
    
    if (cur_node->subtrees[0] != nullptr)
    {
        cur_node = cur_node->subtrees[cur_pos + 1];
        
        while (cur_node->subtrees[0] != nullptr)
        {
            _state.emplace(cur_node, -1);
            cur_node = cur_node->subtrees[0];
        }
        
        _state.emplace(cur_node, 0);
        
        return *this;
    }
    
    if (cur_pos + 1 < cur_node->virtual_size)
    {
        ++_state.top().second;
        return *this;
    }
    
    while (cur_pos + 1 == cur_node->virtual_size)
    {
        _state.pop();
        
        if (_state.empty())
        {
            return *this;
        }
        
        cur_node = _state.top().first;
        cur_pos = _state.top().second;
    }
    
    ++_state.top().second;
    
    return *this;
}

template<
    typename tkey,
    typename tvalue>
typename b_tree<tkey, tvalue>::infix_const_iterator b_tree<tkey, tvalue>::infix_const_iterator::operator++(
    int not_used)
{
    infix_iterator iter = *this;
    *this++;
    return iter;
}

template<
    typename tkey,
    typename tvalue>
std::tuple<size_t, size_t, tkey const &, tvalue const &> b_tree<tkey, tvalue>::infix_const_iterator::operator*() const
{
    if (_state.empty())
    {
        throw std::logic_error("attempt to dereference invalid or end iterator");
    }
    
    auto *kvp = _state.top().first->keys_and_values;
    auto pos = _state.top().second;
    
    return std::tuple<size_t, size_t, tkey const &, tvalue const &>(_state.size() - 1, pos, kvp[pos].key, kvp[pos].value);
}

template<
    typename tkey,
    typename tvalue>
b_tree<tkey, tvalue>::infix_const_iterator::infix_const_iterator(
    typename search_tree<tkey, tvalue>::common_node *node)
{
    if (node == nullptr)
    {
        return;
    }
    
    auto *iter = node;
    
    while (iter->subtrees[0] != nullptr)
    {
        _state.emplace(iter, -1);
        iter = iter->subtrees[0];
    }
    
    _state.emplace(iter, 0);
}

template<
    typename tkey,
    typename tvalue>
b_tree<tkey, tvalue>::infix_const_iterator::infix_const_iterator(
    std::stack<std::pair<typename search_tree<tkey, tvalue>::common_node*, int>> path):
        _state(path)
{ }

template<
        typename tkey,
        typename tvalue>
bool b_tree<tkey, tvalue>::infix_reverse_iterator::operator==(
        typename b_tree::infix_reverse_iterator const &other) const noexcept
{
    if (_state.empty() && other._state.empty())
    {
        return true;
    }

    if (_state.empty() ^ other._state.empty())
    {
        return false;
    }

    return _state.top() == other._state.top();
}

template<
        typename tkey,
        typename tvalue>
bool b_tree<tkey, tvalue>::infix_reverse_iterator::operator!=(
        typename b_tree::infix_reverse_iterator const &other) const noexcept
{
    return !(*this == other);
}

template<
        typename tkey,
        typename tvalue>
typename b_tree<tkey, tvalue>::infix_reverse_iterator &b_tree<tkey, tvalue>::infix_reverse_iterator::operator++()
{
    if (_state.empty())
    {
        return *this; // UB
    }

    auto *cur_node = _state.top().first;
    auto cur_pos = _state.top().second;

    if (cur_node->subtrees[cur_node->virtual_size] != nullptr)
    {
        std::cout << "pu\n";

        cur_node = cur_node->subtrees[cur_pos];

        while (cur_node->subtrees[cur_node->virtual_size] != nullptr)
        {
            _state.emplace(cur_node, cur_node->virtual_size + 1); //TODO cur_node->virtual_size
            cur_node = cur_node->subtrees[cur_node->virtual_size];
        }

        _state.emplace(cur_node, cur_node->virtual_size - 1);

        return *this;
    }

    if (cur_pos > 0)
    {
        std::cout << "pu pu\n";
        --_state.top().second;
        return *this;
    }

    if (cur_pos == 0)
    {
        _state.pop();

        if (_state.empty())
        {
            return *this;
        }

        cur_node = _state.top().first;
        cur_pos = _state.top().second;

        std::cout << "pu pu pu\n";
    }

    //--_state.top().second;

    return *this;
}

template<
        typename tkey,
        typename tvalue>
typename b_tree<tkey, tvalue>::infix_reverse_iterator b_tree<tkey, tvalue>::infix_reverse_iterator::operator++(
        int not_used)
{
    infix_reverse_iterator iter = *this;
    *this++;
    return iter;
}

template<
        typename tkey,
        typename tvalue>
std::tuple<size_t, size_t, tkey const &, tvalue &> b_tree<tkey, tvalue>::infix_reverse_iterator::operator*() const
{
    if (_state.empty())
    {
        throw std::logic_error("attempt to dereference invalid or end iterator");
    }

    auto *kvp = _state.top().first->keys_and_values;
    auto pos = _state.top().second;

    return std::tuple<size_t, size_t, tkey const &, tvalue &>(_state.size() - 1, pos, kvp[pos].key, kvp[pos].value);
}

template<
        typename tkey,
        typename tvalue>
b_tree<tkey, tvalue>::infix_reverse_iterator::infix_reverse_iterator(
        typename search_tree<tkey, tvalue>::common_node *node)
{
    if (node == nullptr)
    {
        return;
    }

    auto *iter = node;

    while (iter->subtrees[iter->virtual_size] != nullptr)
    {
        _state.emplace(iter, iter->virtual_size - 1);
        iter = iter->subtrees[iter->virtual_size];
    }

    _state.emplace(iter, iter->virtual_size - 1);
}

template<
        typename tkey,
        typename tvalue>
bool b_tree<tkey, tvalue>::infix_const_reverse_iterator::operator==(
        b_tree::infix_const_reverse_iterator const &other) const noexcept
{
    if (_state.empty() && other._state.empty())
    {
        return true;
    }

    if (_state.empty() ^ other._state.empty())
    {
        return false;
    }

    return _state.top() == other._state.top();
}

template<
        typename tkey,
        typename tvalue>
bool b_tree<tkey, tvalue>::infix_const_reverse_iterator::operator!=(
        b_tree::infix_const_reverse_iterator const &other) const noexcept
{
    return !(*this == other);
}

template<
        typename tkey,
        typename tvalue>
typename b_tree<tkey, tvalue>::infix_const_reverse_iterator &b_tree<tkey, tvalue>::infix_const_reverse_iterator::operator++()
{
    if (_state.empty())
    {
        return *this; // UB
    }

    auto *cur_node = _state.top().first;
    auto cur_pos = _state.top().second;

    if (cur_node->subtrees[cur_node->virtual_size] != nullptr)
    {
        std::cout << "pu\n";

        cur_node = cur_node->subtrees[cur_pos];

        while (cur_node->subtrees[cur_node->virtual_size] != nullptr)
        {
            _state.emplace(cur_node, cur_node->virtual_size); //TODO cur_node->virtual_size
            cur_node = cur_node->subtrees[cur_node->virtual_size];
        }

        _state.emplace(cur_node, cur_node->virtual_size - 1);

        return *this;
    }

    if (cur_pos > 0)
    {
        std::cout << "pu pu\n";
        --_state.top().second;
        return *this;
    }

    if (cur_pos == 0)
    {
        _state.pop();

        if (_state.empty())
        {
            return *this;
        }

        cur_node = _state.top().first;
        cur_pos = _state.top().second;

        std::cout << "pu pu pu\n";
    }

    //--_state.top().second;

    return *this;
}

template<
        typename tkey,
        typename tvalue>
typename b_tree<tkey, tvalue>::infix_const_reverse_iterator b_tree<tkey, tvalue>::infix_const_reverse_iterator::operator++(
        int not_used)
{
    infix_reverse_iterator iter = *this;
    *this++;
    return iter;
}

template<
        typename tkey,
        typename tvalue>
std::tuple<size_t, size_t, tkey const &, tvalue const &> b_tree<tkey, tvalue>::infix_const_reverse_iterator::operator*() const
{
    if (_state.empty())
    {
        throw std::logic_error("attempt to dereference invalid or end iterator");
    }

    auto *kvp = _state.top().first->keys_and_values;
    auto pos = _state.top().second;

    return std::tuple<size_t, size_t, tkey const &, tvalue const &>(_state.size() - 1, pos, kvp[pos].key, kvp[pos].value);
}

template<
        typename tkey,
        typename tvalue>
b_tree<tkey, tvalue>::infix_const_reverse_iterator::infix_const_reverse_iterator(
        typename search_tree<tkey, tvalue>::common_node *node)
{
    if (node == nullptr)
    {
        return;
    }

    auto *iter = node;

    while (iter->subtrees[iter->virtual_size] != nullptr)
    {
        _state.emplace(iter, iter->virtual_size - 1);
        iter = iter->subtrees[iter->virtual_size];
    }

    _state.emplace(iter, iter->virtual_size - 1);
}

template<
        typename tkey,
        typename tvalue>
b_tree<tkey, tvalue>::infix_const_reverse_iterator::infix_const_reverse_iterator(
        std::stack<std::pair<typename search_tree<tkey, tvalue>::common_node*, int>> path):
        _state(path)
{ }

#pragma endregion infix iterator implementation

#pragma region BTree CRUD imlementation

template<
    typename tkey,
    typename tvalue>
void b_tree<tkey, tvalue>::insert_inner(
    typename associative_container<tkey, tvalue>::key_value_pair &&kvp,
    bool is_update)
{
    auto path = this->find_path(kvp.key);
    
    if (path.top().second >= 0)
    {
        if (is_update)
        {
            (*path.top().first)->keys_and_values[path.top().second] = std::move(kvp);
        }
        else
        {
            throw typename search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_exception_exception(kvp.key);
        }
        
        return;
    }
    
    if (is_update)
    {
        throw typename search_tree<tkey, tvalue>::updating_of_nonexistent_key_attempt_exception(kvp.key);
    }
    
    if (*path.top().first == nullptr && path.size() == 1)
    {
        typename search_tree<tkey, tvalue>::common_node *new_node = this->create_node(_t);
        *path.top().first = new_node;
        allocator::construct(new_node->keys_and_values, std::move(kvp));
        ++new_node->virtual_size;
        
        return;
    }
    
    typename search_tree<tkey, tvalue>::common_node *node = *path.top().first;
    typename search_tree<tkey, tvalue>::common_node *right_subtree = nullptr;
    size_t subtree_index = -path.top().second - 1;
    
    while(true)
    {
        if (node->virtual_size < get_max_keys_count())
        {
            this->node_insert(node, std::move(kvp), subtree_index, right_subtree);
            return;
        }
        
        auto pair = this->node_split(node, std::move(kvp), subtree_index, right_subtree);
        right_subtree = pair.first;
        kvp = std::move(pair.second);
        
        
        if (path.size() == 1)
        {
            typename search_tree<tkey, tvalue>::common_node *new_root = this->create_node(_t);
            allocator::construct(new_root->keys_and_values, std::move(kvp));
            new_root->virtual_size = 1;
            new_root->subtrees[0] = node;
            new_root->subtrees[1] = right_subtree;
            *path.top().first = new_root;
            return;
        }
        
        path.pop();
        node = *path.top().first;
        subtree_index = -path.top().second - 1;
    }
}

template<
    typename tkey,
    typename tvalue>
void b_tree<tkey, tvalue>::insert(
    tkey const &key,
    tvalue const &value)
{
    std::lock_guard<std::mutex> lock(_mutex);
    
    this->trace_with_guard(get_typename() + "::insert(tkey const &, tvalue const &) : called.")
        ->debug_with_guard(get_typename() + "::insert(tkey const &, tvalue const &) : called.")
        ->debug_with_guard(get_typename() + "::insert(tkey const &, tvalue const &) : inserting node with key \"" + 
            extra_utility::make_string(key) + "\"");
    
    try
    {
        insert_inner(std::move(typename associative_container<tkey, tvalue>::key_value_pair(key, value)), false);
    }
    catch (typename search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_exception_exception const &)
    {
        this->error_with_guard(get_typename() + "::insert(tkey const &, tvalue const &) : attempt to insert key duplicate.");
        throw;
    }
    catch (std::bad_alloc const &)
    {
        this->error_with_guard(get_typename() + "::insert(tkey const &, tvalue const &) : bad alloc occurred.");
        throw;
    }
    
    this->trace_with_guard(get_typename() + "::insert(tkey const &, tvalue const &) : successfuly finished.")
        ->debug_with_guard(get_typename() + "::insert(tkey const &, tvalue const &) : successfuly finished.");
}

template<
    typename tkey,
    typename tvalue>
void b_tree<tkey, tvalue>::insert(
    tkey const &key,
    tvalue &&value)
{
    std::lock_guard<std::mutex> lock(_mutex);
    
    this->trace_with_guard(get_typename() + "::insert(tkey const &, tvalue &&) : called.")
        ->debug_with_guard(get_typename() + "::insert(tkey const &, tvalue &&) : called.")
        ->debug_with_guard(get_typename() + "::insert(tkey const &, tvalue &&) : inserting node with key \"" + 
            extra_utility::make_string(key) + "\"");
    
    try
    {
        insert_inner(std::move(typename associative_container<tkey, tvalue>::key_value_pair(key, std::move(value))), false);
    }
    catch (typename search_tree<tkey, tvalue>::insertion_of_existent_key_attempt_exception_exception const &)
    {
        this->error_with_guard(get_typename() + "::insert(tkey const &, tvalue const &) : attempt to insert key duplicate.");
        throw;
    }
    catch (std::bad_alloc const &)
    {
        this->error_with_guard(get_typename() + "::insert(tkey const &, tvalue const &) : bad alloc occurred.");
        throw;
    }
    
    this->trace_with_guard(get_typename() + "::insert(tkey const &, tvalue &&) : successfuly finished.")
        ->debug_with_guard(get_typename() + "::insert(tkey const &, tvalue &&) : successfuly finished.");
}

template<
    typename tkey,
    typename tvalue>
void b_tree<tkey, tvalue>::update(
    tkey const &key,
    tvalue const &value)
{
    std::lock_guard<std::mutex> lock(_mutex);
    
    this->trace_with_guard(get_typename() + "::insert(tkey const &, tvalue const &) : called.")
        ->debug_with_guard(get_typename() + "::insert(tkey const &, tvalue const &) : called.")
        ->debug_with_guard(get_typename() + "::insert(tkey const &, tvalue const &) : inserting node with key \"" + 
            extra_utility::make_string(key) + "\"");
    
    try
    {
        insert_inner(std::move(typename associative_container<tkey, tvalue>::key_value_pair(key, value)), true);
    }
    catch (typename search_tree<tkey, tvalue>::updating_of_nonexistent_key_attempt_exception const &)
    {
        this->error_with_guard(get_typename() + "::insert(tkey const &, tvalue const &) : attempt to update value by non-existent key.");
        throw;
    }
    catch (std::bad_alloc const &)
    {
        this->error_with_guard(get_typename() + "::insert(tkey const &, tvalue const &) : bad alloc occurred.");
        throw;
    }
    
    this->trace_with_guard(get_typename() + "::insert(tkey const &, tvalue const &) : successfuly finished.")
        ->debug_with_guard(get_typename() + "::insert(tkey const &, tvalue const &) : successfuly finished.");
}

template<
    typename tkey,
    typename tvalue>
void b_tree<tkey, tvalue>::update(
    tkey const &key,
    tvalue &&value)
{
    std::lock_guard<std::mutex> lock(_mutex);
    
    this->trace_with_guard(get_typename() + "::insert(tkey const &, tvalue &&) : called.")
        ->debug_with_guard(get_typename() + "::insert(tkey const &, tvalue &&) : called.")
        ->debug_with_guard(get_typename() + "::insert(tkey const &, tvalue &&) : inserting node with key \"" + 
            extra_utility::make_string(key) + "\"");
    
    try
    {
        insert_inner(std::move(typename associative_container<tkey, tvalue>::key_value_pair(key, std::move(value))), true);
    }
    catch (typename search_tree<tkey, tvalue>::updating_of_nonexistent_key_attempt_exception const &)
    {
        this->error_with_guard(get_typename() + "::insert(tkey const &, tvalue const &) : attempt to update value by non-existent key.");
        throw;
    }
    catch (std::bad_alloc const &)
    {
        this->error_with_guard(get_typename() + "::insert(tkey const &, tvalue const &) : bad alloc occurred.");
        throw;
    }
    
    this->trace_with_guard(get_typename() + "::insert(tkey const &, tvalue &&) : successfuly finished.")
        ->debug_with_guard(get_typename() + "::insert(tkey const &, tvalue &&) : successfuly finished.");
}

template<
    typename tkey,
    typename tvalue>
tvalue &b_tree<tkey, tvalue>::obtain(
    tkey const &key)
{
    std::lock_guard<std::mutex> lock(_mutex);
    
    this->trace_with_guard(get_typename() + "::obtain(tkey const &) : called.")
        ->debug_with_guard(get_typename() + "::obtain(tkey const &) : called.");
    
    auto path = this->find_path(key);
    if (path.top().second < 0)
    {
        this->error_with_guard(get_typename() + "::obtain(tkey const &) : key \"" +
                extra_utility::make_string(key) + "\" is not present in container.");
        throw typename search_tree<tkey, tvalue>::obtaining_of_nonexistent_key_attempt_exception(key);
    }
    
    this->trace_with_guard(get_typename() + "::obtain(tkey const &) : successfuly finished.")
        ->debug_with_guard(get_typename() + "::obtain(tkey const &) : successfuly finished.");
    
    return (*path.top().first)->keys_and_values[path.top().second].value;
}

template<
    typename tkey,
    typename tvalue>
tvalue b_tree<tkey, tvalue>::dispose(
    tkey const &key)
{
    std::lock_guard<std::mutex> lock(_mutex);
    
    this->trace_with_guard(get_typename() + "::dispose(tkey const &) : called.")
        ->debug_with_guard(get_typename() + "::dispose(tkey const &) : called.")
        ->debug_with_guard(get_typename() + "::dispose(tkey const &) : disposing node with key \"" + 
            extra_utility::make_string(key) + "\"");
    
    auto path = this->find_path(key);
    if (path.top().second < 0)
    {
        this->error_with_guard(get_typename() + "::dispose(tkey const &) : key \"" +
                extra_utility::make_string(key) + "\" is not present in container.");
        throw typename search_tree<tkey, tvalue>::disposal_of_nonexistent_key_attempt_exception(key);
    }
    
    // Reducing non-leaf disposal to leaf disposal
    if ((*path.top().first)->subtrees[0] != nullptr)
    {
        auto *non_leaf = *path.top().first;
        auto non_leaf_index = path.top().second;
        typename search_tree<tkey, tvalue>::common_node **iterator = non_leaf->subtrees + non_leaf_index;
        path.top().second = -non_leaf_index - 1;
        
        // TODO: configure this for min of right subtree (what for???)
        while (*iterator != nullptr)
        {
            auto index = (*iterator)->virtual_size;
            path.emplace(iterator, -index - 1);
            iterator = (*iterator)->subtrees + index;
        }
        
        auto *leaf = *path.top().first;
        auto &leaf_index = path.top().second;
        
        leaf_index = leaf->virtual_size - 1;
        std::swap(non_leaf->keys_and_values[non_leaf_index], leaf->keys_and_values[leaf_index]);
    }
    
    auto *target_node = *path.top().first;
    auto kvp_to_dispose_index = path.top().second;
    path.pop();
    
    tvalue value = std::move(target_node->keys_and_values[kvp_to_dispose_index].value);
    
    for (size_t i = kvp_to_dispose_index + 1; i < target_node->virtual_size; ++i)
    {
        std::swap(target_node->keys_and_values[i - 1], target_node->keys_and_values[i]);
    }
    
    --target_node->virtual_size;
    allocator::destruct(target_node->keys_and_values + target_node->virtual_size);
    
    while (true)
    {
        if (target_node->virtual_size >= get_min_keys_count())
        {
            this->trace_with_guard(get_typename() + "::dispose(tkey const &) : successfuly finished.")
                ->debug_with_guard(get_typename() + "::dispose(tkey const &) : successfuly finished.");
            return value;
        }
        
        if (path.size() == 0)
        {
            if (target_node->virtual_size == 0)
            {
                this->_root = target_node->subtrees[0];
                this->destroy_node(target_node);
                this->debug_with_guard(get_typename() + "::dispose(tkey const &) : new root is set.");
            }
            
            this->trace_with_guard(get_typename() + "::dispose(tkey const &) : successfuly finished.")
                ->debug_with_guard(get_typename() + "::dispose(tkey const &) : successfuly finished.");
            return value;
        }
        
        // right parent
        typename search_tree<tkey, tvalue>::common_node *parent = *path.top().first;
        size_t parent_index = -path.top().second - 1;
        path.pop();
        
        bool const left_brother_exists = parent_index != 0;
        bool const can_take_from_left = left_brother_exists && parent->subtrees[parent_index-1]->virtual_size > get_min_keys_count();
        
        
        bool const right_brother_exists = parent_index != parent->virtual_size;
        bool const can_take_from_right = right_brother_exists && parent->subtrees[parent_index+1]->virtual_size > get_min_keys_count();
        
        if (can_take_from_left)
        {
            auto *left_brother = parent->subtrees[parent_index - 1];
            std::swap(parent->keys_and_values[parent_index-1], left_brother->keys_and_values[left_brother->virtual_size-1]);
            
            allocator::construct(target_node->keys_and_values + target_node->virtual_size,
                    std::move(left_brother->keys_and_values[left_brother->virtual_size-1]));
            ++target_node->virtual_size;
            
            target_node->subtrees[target_node->virtual_size] = target_node->subtrees[target_node->virtual_size - 1];
            target_node->subtrees[target_node->virtual_size - 1] = left_brother->subtrees[left_brother->virtual_size];
            
            for (size_t i = target_node->virtual_size - 1; i > 0; --i)
            {
                std::swap(target_node->keys_and_values[i], target_node->keys_and_values[i-1]);
                std::swap(target_node->subtrees[i], target_node->subtrees[i-1]);
            }
            
            --left_brother->virtual_size;
            allocator::destruct(left_brother->keys_and_values + left_brother->virtual_size);
            
            this->trace_with_guard(get_typename() + "::dispose(tkey const &) : successfuly finished.")
                ->debug_with_guard(get_typename() + "::dispose(tkey const &) : successfuly finished.");
            return value;
        }
        
        if (can_take_from_right)
        {
            auto *right_brother = parent->subtrees[parent_index + 1];
            std::swap(parent->keys_and_values[parent_index], right_brother->keys_and_values[0]);
            
            allocator::construct(target_node->keys_and_values + target_node->virtual_size, std::move(right_brother->keys_and_values[0]));
            ++target_node->virtual_size;
            
            target_node->subtrees[target_node->virtual_size] = right_brother->subtrees[0];
            
            for (size_t i = 0; i < right_brother->virtual_size - 1; ++i)
            {
                std::swap(right_brother->keys_and_values[i], right_brother->keys_and_values[i+1]);
                right_brother->subtrees[i] = right_brother->subtrees[i+1];
            }
            
            right_brother->subtrees[right_brother->virtual_size-1] = right_brother->subtrees[right_brother->virtual_size];
            
            --right_brother->virtual_size;
            allocator::destruct(right_brother->keys_and_values + right_brother->virtual_size);
            
            this->trace_with_guard(get_typename() + "::dispose(tkey const &) : successfuly finished.")
                ->debug_with_guard(get_typename() + "::dispose(tkey const &) : successfuly finished.");
            return value;
        }
        
        this->node_merge(parent, parent_index - (left_brother_exists ? 1 : 0));
        
        target_node = parent;
    }
}

template<
    typename tkey,
    typename tvalue>
std::vector<typename associative_container<tkey, tvalue>::key_value_pair> b_tree<tkey, tvalue>::obtain_between(
    tkey const &lower_bound,
    tkey const &upper_bound,
    bool lower_bound_inclusive,
    bool upper_bound_inclusive)
{
    std::lock_guard<std::mutex> lock(_mutex);

    this->trace_with_guard(get_typename() + "::obtain_between(tkey const &, tkey const &, bool, bool) : called.")
        ->debug_with_guard(get_typename() + "::obtain_between(tkey const &, tkey const &, bool, bool) : called.");

    auto const &comparer = this->_keys_comparer;
    std::vector<typename associative_container<tkey, tvalue>::key_value_pair> range;
    std::stack<std::pair<typename search_tree<tkey, tvalue>::common_node *, int>> path;

    auto *path_finder = reinterpret_cast<typename search_tree<tkey, tvalue>::common_node *>(this->_root);

    while (path_finder != nullptr)
    {
        int index = this->node_find_path(path_finder, lower_bound, 0, path_finder->virtual_size - 1);
        if (index >= 0)
        {
            path.emplace(path_finder, index);
            break;
        }

        path.emplace(path_finder, -index - 2);

        path_finder = path_finder->subtrees[-index - 1];
    }

    if (path.top().second == -1)
    {
        path.top().second = 0;
    }

    if (path.empty())
    {
        return range;
    }

    tkey &found_key = path.top().first->keys_and_values[path.top().second].key;
    auto iter = infix_const_iterator(path);
    auto end_iter = infix_const_iterator(nullptr);

    if (comparer(found_key, lower_bound) < (lower_bound_inclusive ? 0 : 1))
    {
        ++iter;
    }

    while(iter != end_iter &&
            (comparer(std::get<2>(*iter), upper_bound)) < (upper_bound_inclusive ? 1 : 0))
    {
        range.emplace_back(std::get<2>(*iter), std::get<3>(*iter));
        ++iter;
    }

    this->trace_with_guard(get_typename() + "::obtain_between(tkey const &, tkey const &, bool, bool) : successfuly finished.")
        ->debug_with_guard(get_typename() + "::obtain_between(tkey const &, tkey const &, bool, bool) : successfuly finished.");


    return range;
}

#pragma endregion BTree CRUD imlementation

#pragma region BTree construction, assignment, destruction implementation

template<
    typename tkey,
    typename tvalue>
b_tree<tkey, tvalue>::b_tree(
    size_t t,
    std::function<int(tkey const &, tkey const &)> keys_comparer,
    allocator *allocator,
    logger *logger):
        search_tree<tkey, tvalue>(keys_comparer, allocator, logger),
        _t(t)
{
    if (t < 2)
    {
        throw std::logic_error("parameter t must be not less than 2");
    }
}

template<
    typename tkey,
    typename tvalue>
b_tree<tkey, tvalue>::b_tree(
    b_tree<tkey, tvalue> const &other):
        search_tree<tkey, tvalue>(other._keys_comparer, other._allocator, other._logger),
        _t(other._t)
{
    std::lock(_mutex, other._mutex);
    std::lock_guard<std::mutex> lock_1(_mutex, std::adopt_lock);
    std::lock_guard<std::mutex> lock_2(other._mutex, std::adopt_lock);
    
    try
    {
        this->_root = copy(reinterpret_cast<typename search_tree<tkey, tvalue>::common_node *>(other._root));
    }
    catch (const std::bad_alloc& ex)
    {
        throw;
    }
}

template<
    typename tkey,
    typename tvalue>
b_tree<tkey, tvalue>::b_tree(
    b_tree<tkey, tvalue> &&other) noexcept:
        search_tree<tkey, tvalue>(other._keys_comparer, other._allocator, other._logger),
        _t(other._t)
{
    std::lock(_mutex, other._mutex);
    std::lock_guard<std::mutex> lock_1(_mutex, std::adopt_lock);
    std::lock_guard<std::mutex> lock_2(other._mutex, std::adopt_lock);
    
    this->_root = other._root;
    
    other._logger = nullptr;
    other._allocator = nullptr;
    other._root = nullptr;
}

template<
    typename tkey,
    typename tvalue>
b_tree<tkey, tvalue> &b_tree<tkey, tvalue>::operator=(
    b_tree<tkey, tvalue> const &other)
{
    std::lock(_mutex, other._mutex);
    std::lock_guard<std::mutex> lock_1(_mutex, std::adopt_lock);
    std::lock_guard<std::mutex> lock_2(other._mutex, std::adopt_lock);
    
    if (this != &other)
    {
        this->clear(reinterpret_cast<typename search_tree<tkey, tvalue>::common_node *>(this->_root));
        this->_root = copy(reinterpret_cast<typename search_tree<tkey, tvalue>::common_node *>(other._root));
        
        this->_keys_comparer = other._keys_comparer;
        this->_allocator = other._allocator;
        this->_logger = other._logger;
        
        _t = other._t;
    }
    
    return *this;
}

template<
    typename tkey,
    typename tvalue>
b_tree<tkey, tvalue> &b_tree<tkey, tvalue>::operator=(
    b_tree<tkey, tvalue> &&other) noexcept
{
    std::lock(_mutex, other._mutex);
    std::lock_guard<std::mutex> lock_1(_mutex, std::adopt_lock);
    std::lock_guard<std::mutex> lock_2(other._mutex, std::adopt_lock);
    
    if (this != &other)
    {
        this->clear(reinterpret_cast<typename search_tree<tkey, tvalue>::common_node *>(this->_root));
        
        this->_keys_comparer = std::move(other._keys_comparer);
        this->_allocator = other._allocator;
        this->_logger = other._logger;
        this->_root = other._root;
        
        _t = other._t;
        
        other._allocator = nullptr;
        other._logger = nullptr;
        other._root = nullptr;
    }
    
    return *this;
}

#pragma endregion BTree construction, assignment, destruction implementation

#pragma region iterators requesting implementation

template<
    typename tkey,
    typename tvalue>
typename b_tree<tkey, tvalue>::infix_iterator b_tree<tkey, tvalue>::begin_infix() const noexcept
{
    return infix_iterator(reinterpret_cast<typename search_tree<tkey, tvalue>::common_node *>(this->_root));
}

template<
    typename tkey,
    typename tvalue>
typename b_tree<tkey, tvalue>::infix_iterator b_tree<tkey, tvalue>::end_infix() const noexcept
{
    return infix_iterator(nullptr);
}

template<
    typename tkey,
    typename tvalue>
typename b_tree<tkey, tvalue>::infix_const_iterator b_tree<tkey, tvalue>::cbegin_infix() const noexcept
{
    return infix_const_iterator(reinterpret_cast<typename search_tree<tkey, tvalue>::common_node *>(this->_root));
}

template<
    typename tkey,
    typename tvalue>
typename b_tree<tkey, tvalue>::infix_const_iterator b_tree<tkey, tvalue>::cend_infix() const noexcept
{
    return infix_const_iterator(nullptr);
}

template<
        typename tkey,
        typename tvalue>
typename b_tree<tkey, tvalue>::infix_reverse_iterator b_tree<tkey, tvalue>::rbegin_infix() const noexcept
{
    return infix_reverse_iterator(reinterpret_cast<typename search_tree<tkey, tvalue>::common_node *>(this->_root));
}

template<
        typename tkey,
        typename tvalue>
typename b_tree<tkey, tvalue>::infix_reverse_iterator b_tree<tkey, tvalue>::rend_infix() const noexcept
{
    return infix_reverse_iterator(nullptr);
}

template<
        typename tkey,
        typename tvalue>
typename b_tree<tkey, tvalue>::infix_const_reverse_iterator b_tree<tkey, tvalue>::crbegin_infix() const noexcept
{
    return infix_const_reverse_iterator(reinterpret_cast<typename search_tree<tkey, tvalue>::common_node *>(this->_root));
}

template<
        typename tkey,
        typename tvalue>
typename b_tree<tkey, tvalue>::infix_const_reverse_iterator b_tree<tkey, tvalue>::crend_infix() const noexcept
{
    return infix_const_reverse_iterator(nullptr);
}

#pragma endregion iterators requesting implementation

#pragma region BTree extra functions

template<
    typename tkey,
    typename tvalue>
size_t b_tree<tkey, tvalue>::get_min_keys_count() const noexcept
{
    return _t - 1;
}

template<
    typename tkey,
    typename tvalue>
size_t b_tree<tkey, tvalue>::get_max_keys_count() const noexcept
{
    return 2 * _t - 1;
}

template<
    typename tkey,
    typename tvalue>
typename search_tree<tkey, tvalue>::common_node *b_tree<tkey, tvalue>::copy(
    typename search_tree<tkey, tvalue>::common_node const *node)
{
    if (node == nullptr)
    {
        return nullptr;
    }
    
    typename search_tree<tkey, tvalue>::common_node *copied = this->create_node(_t);
    copied->virtual_size = node->virtual_size;
    
    for (size_t i = 0; i <= node->virtual_size; ++i)
    {
        copied->subtrees[i] = nullptr;
    }
       
    try
    {
        for (size_t i = 0; i <= node->virtual_size; ++i)
        {
            copied->subtrees[i] = copy(node->subtrees[i]);
        }
    }
    catch (std::bad_alloc const &)
    {
        clear(copied);
        throw;
    }
    
    for (size_t i = 0; i < node->virtual_size; ++i)
    {
        allocator::construct(copied->keys_and_values + i, node->keys_and_values[i]);
    }
    
    return copied;
}

template<
    typename tkey,
    typename tvalue>
void b_tree<tkey, tvalue>::clear(
    typename search_tree<tkey, tvalue>::common_node *node)
{
    if (node == nullptr)
    {
        return;
    }
    
    for (size_t i = 0; i <= node->virtual_size; ++i)
    {
        clear(node->subtrees[i]);
    }

    this->destroy_node(node);
}

#pragma endregion BTree extra functions

template<
    typename tkey,
    typename tvalue>
inline std::string b_tree<tkey, tvalue>::get_typename() const noexcept
{
    return "b_tree<tkey, tvalue>";
}

#endif //MATH_PRACTICE_AND_OPERATING_SYSTEMS_TEMPLATE_REPO_B_TREE_H