cmake_minimum_required(VERSION 3.21)
project(os_cw_allctr_allctr_glbl_hp)

add_library(
        os_cw_allctr_allctr_glbl_hp
        src/allocator_global_heap.cpp)
target_include_directories(
        os_cw_allctr_allctr_glbl_hp
        PUBLIC
        ./include)
target_link_libraries(
        os_cw_allctr_allctr_glbl_hp
        PUBLIC
        os_cw_cmmn)
target_link_libraries(
        os_cw_allctr_allctr_glbl_hp
        PUBLIC
        os_cw_lggr_lggr)
target_link_libraries(
        os_cw_allctr_allctr_glbl_hp
        PUBLIC
        os_cw_allctr_allctr)
set_target_properties(
        os_cw_allctr_allctr_glbl_hp PROPERTIES
        LANGUAGES CXX
        LINKER_LANGUAGE CXX
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
        CXX_EXTENSIONS OFF
        VERSION 1.0
        DESCRIPTION "global heap allocator implementation library")