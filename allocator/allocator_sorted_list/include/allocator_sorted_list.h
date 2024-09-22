#ifndef OS_CW_ALLOCATOR_SORTED_LIST_H
#define OS_CW_ALLOCATOR_SORTED_LIST_H

#include <allocator_guardant.h>
#include <allocator_test_utils.h>
#include <allocator_with_fit_mode.h>
#include <logger_guardant.h>
#include <typename_holder.h>

#include <mutex>

class allocator_sorted_list final:
        private allocator_guardant,
        public allocator_test_utils,
        public allocator_with_fit_mode,
        private logger_guardant,
        private typename_holder
{

private:

    void* _trusted_memory;

public:

    ~allocator_sorted_list() override;

    allocator_sorted_list(
            allocator_sorted_list const &other) = delete;

    allocator_sorted_list &operator=(
            allocator_sorted_list const &other) = delete;

    allocator_sorted_list(
            allocator_sorted_list &&other) noexcept;

    allocator_sorted_list &operator=(
            allocator_sorted_list &&other) noexcept;

public:

    explicit allocator_sorted_list(
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

    inline logger *get_logger() const override;

    inline allocator_with_fit_mode::fit_mode &get_fit_mode() const;

    inline std::mutex &get_mutex() const;

    inline allocator::block_size_t get_allocator_size() const;

    inline allocator::block_size_t &get_free_space() const;

public:

    std::vector<allocator_test_utils::block_info> get_blocks_info() const noexcept override;

private:

    std::string get_block_dump(block_pointer_t block, block_size_t size);

    void debug_blocks_info(std::string call_function_name) const;

    std::vector<allocator_test_utils::block_info> create_blocks_info() const noexcept;

private:

    inline allocator::block_size_t get_meta_size() const;

    inline allocator::block_size_t get_block_meta_size() const;

    inline allocator::block_size_t get_occupied_meta_size() const;

private:

    inline allocator::block_pointer_t &get_head_block() const;

    inline allocator::block_pointer_t &get_next_block(
            block_pointer_t block) const;

    inline allocator::block_size_t &get_block_size(
            block_pointer_t block) const;

    inline allocator *&get_block_allocator(
            block_pointer_t block) const;

private:

    inline std::string get_typename() const noexcept override;

};


#endif //OS_CW_ALLOCATOR_SORTED_LIST_H
