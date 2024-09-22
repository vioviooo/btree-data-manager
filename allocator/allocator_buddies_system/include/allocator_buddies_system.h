#ifndef OS_CW_ALLOCATOR_BUDDIES_SYSTEM_H
#define OS_CW_ALLOCATOR_BUDDIES_SYSTEM_H

#include <allocator_guardant.h>
#include <allocator_test_utils.h>
#include <allocator_with_fit_mode.h>
#include <logger_guardant.h>
#include <typename_holder.h>
#include <mutex>

class allocator_buddies_system final:
        private allocator_guardant,
        public allocator_test_utils,
        public allocator_with_fit_mode,
        private logger_guardant,
        private typename_holder
{

private:

    void *_trusted_memory;

public:

    ~allocator_buddies_system() override;

    allocator_buddies_system(
            allocator_buddies_system const &other) = delete;

    allocator_buddies_system &operator=(
            allocator_buddies_system const &other) = delete;

    allocator_buddies_system(
            allocator_buddies_system &&other) noexcept;

    allocator_buddies_system &operator=(
            allocator_buddies_system &&other) noexcept;

public:

    explicit allocator_buddies_system(
            size_t space_size_power_of_two,
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

public:

    std::vector<allocator_test_utils::block_info> get_blocks_info() const noexcept override;

private:
    inline allocator *get_allocator() const override;
    inline logger *get_logger() const override;
    inline std::string get_typename() const noexcept override;
    inline fit_mode &get_fit_mode() const;
    inline std::mutex &get_mutex() const;

private:
    inline block_size_t get_available_block_meta_size() const;
    inline block_size_t get_occupied_block_meta_size() const;
    inline block_size_t &get_allocator_available_size() const;
    inline block_size_t get_allocator_meta_size() const;
    inline unsigned char get_allocator_data_size() const;

private:
    inline std::vector<allocator_test_utils::block_info> create_blocks_info() const noexcept;
    void log_blocks_info(const std::string &func_name) const;

private:
    inline block_pointer_t &get_first_available_block() const;
    inline block_pointer_t &get_next_available_block(block_pointer_t block) const;
    inline block_pointer_t &get_prev_available_block(block_pointer_t block) const;
    inline bool block_is_occupied(block_pointer_t block) const noexcept;
    inline allocator *&get_block_allocator(block_pointer_t block) const;
    inline block_pointer_t get_buddy(allocator::block_pointer_t block) const;

private:
    inline unsigned char get_block_data_size(block_pointer_t block) const;
    inline unsigned char &set_block_size(block_pointer_t block) const;
    inline void occupy_block(allocator::block_pointer_t block) const;
    inline void avail_block(allocator::block_pointer_t block) const;

private:
    unsigned char get_power_of_size(block_size_t size) const;
    inline bool belong_trusted_memory(block_pointer_t block) const;
};

#endif //OS_CW_ALLOCATOR_BUDDIES_SYSTEM_H
