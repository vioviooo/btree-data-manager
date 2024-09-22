cmake_minimum_required(VERSION 3.21)
project(os_cw_allctr_allctr_buds_ssm)

add_library(
        os_cw_allctr_allctr_buds_ssm
        src/allocator_buddies_system.cpp
        include/allocator_buddies_system.h)
target_include_directories(
        os_cw_allctr_allctr_buds_ssm
        PUBLIC
        ./include)
target_link_libraries(
        os_cw_allctr_allctr_buds_ssm
        PUBLIC
        os_cw_cmmn)
target_link_libraries(
        os_cw_allctr_allctr_buds_ssm
        PUBLIC
        os_cw_lggr_lggr)
target_link_libraries(
        os_cw_allctr_allctr_buds_ssm
        PUBLIC
        os_cw_allctr_allctr)
set_target_properties(
        os_cw_allctr_allctr_buds_ssm PROPERTIES
        LANGUAGES CXX
        LINKER_LANGUAGE CXX
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
        CXX_EXTENSIONS OFF
        VERSION 1.0
        DESCRIPTION "buddies system allocator implementation library")