cmake_minimum_required(VERSION 3.21)
project(os_cw_cmmn)

add_library(
        os_cw_cmmn
        src/extra_utility.cpp
        src/file_cannot_be_opened.cpp
        src/not_implemented.cpp
        src/operation_not_supported.cpp)
target_include_directories(
        os_cw_cmmn
        PUBLIC
        ./include)
set_target_properties(
        os_cw_cmmn PROPERTIES
        LANGUAGES CXX
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
        CXX_EXTENSIONS OFF
        VERSION 1.0
        DESCRIPTION "common utils and exception classes library")