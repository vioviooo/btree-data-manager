#ifndef OS_CW_ALLOCATOR_RED_BLACK_TREE_H
#define OS_CW_ALLOCATOR_RED_BLACK_TREE_H


#include <allocator_guardant.h>
#include <allocator_test_utils.h>
#include <allocator_with_fit_mode.h>
#include <logger_guardant.h>
#include <typename_holder.h>

class allocator_red_black_tree final:
        private allocator_guardant,
        public allocator_test_utils,
        public allocator_with_fit_mode,
        private logger_guardant,
        private typename_holder
{

private:

    void *_trusted_memory;

public:

    ~allocator_red_black_tree() override;

    allocator_red_black_tree(
            allocator_red_black_tree const &other) = delete;

    allocator_red_black_tree &operator=(
            allocator_red_black_tree const &other) = delete;

    allocator_red_black_tree(
            allocator_red_black_tree &&other) noexcept;

    allocator_red_black_tree &operator=(
            allocator_red_black_tree &&other) noexcept;

public:

    explicit allocator_red_black_tree(
            size_t space_size,
            allocator *parent_allocator = nullptr,
            logger *logger = nullptr,
            allocator_with_fit_mode::fit_mode allocate_fit_mode = allocator_with_fit_mode::fit_mode::first_fit);

public:

    [[nodiscard]] void *allocate(
            size_t value_size,
            size_t values_count) override;

    void deallocate(
            void *at) override;

public:

    inline void set_fit_mode(
            allocator_with_fit_mode::fit_mode mode) override;

private:

    inline allocator *get_allocator() const override;

public:

    std::vector<allocator_test_utils::block_info> get_blocks_info() const noexcept override;

private:

    inline logger *get_logger() const override;
    inline std::mutex &get_mutex() const;
    inline fit_mode &get_fit_mode() const;

private:

    inline std::string get_typename() const noexcept override;

private:
    static inline block_pointer_t &get_prev_block(block_pointer_t block) ;
    static inline block_pointer_t &get_next_block(block_pointer_t block) ;
    inline block_pointer_t &get_root_block() const;
    static inline block_pointer_t &get_parent(block_pointer_t block) ;
    inline block_pointer_t &get_uncle(block_pointer_t block) const;
    static inline block_pointer_t &get_left_child(block_pointer_t block) ;
    static inline block_pointer_t &get_right_child(block_pointer_t block) ;

    static inline block_size_t get_available_block_meta_size() ;
    static inline block_size_t get_occupied_block_meta_size() ;

    static block_size_t &get_block_data_size(block_pointer_t block) ;
    static inline allocator *&get_block_allocator(block_pointer_t block) ;

    static inline bool block_is_occupied(block_pointer_t block) noexcept;
    static inline void occupy_block(block_pointer_t block) ;
    static inline void avail_block(block_pointer_t block) ;

    static inline bool block_is_black(block_pointer_t block) noexcept;
    static inline void set_black(block_pointer_t block) ;
    static inline void set_red(block_pointer_t block) ;



private:
    inline block_size_t &get_allocator_data_size() const;
    static inline block_size_t get_allocator_meta_size() ;

private:
    block_pointer_t get_first_fit(block_size_t size);
    block_pointer_t get_worst_fit(block_size_t size);
    block_pointer_t get_best_fit(block_size_t size);

private:
    void insert_to_tree(block_pointer_t block, size_t size);
    void delete_from_tree(block_pointer_t block, size_t size);
    void balance_after_insert(block_pointer_t new_block);
    void balance_after_delete(block_pointer_t new_block, block_pointer_t parent, block_pointer_t old_block);
    void balance_black_leaf(block_pointer_t new_block, block_pointer_t parent);
    void left_rotate(block_pointer_t new_block);
    void right_rotate(block_pointer_t new_block);

};


#endif //OS_CW_ALLOCATOR_RED_BLACK_TREE_H
