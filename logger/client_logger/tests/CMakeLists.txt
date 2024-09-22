cmake_minimum_required(VERSION 3.21)
project(os_cw_lggr_clnt_lggr_tests)

include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip)

# For Windows users: prevent overriding the parent project's compiler/linker settings
# set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(
        googletest)

add_executable(
        os_cw_lggr_clnt_lggr_tests
        client_logger_tests.cpp)
target_link_libraries(
        os_cw_lggr_clnt_lggr_tests
        PRIVATE
        gtest_main)
target_link_libraries(
        os_cw_lggr_clnt_lggr_tests
        PUBLIC
        os_cw_cmmn)
target_link_libraries(
        os_cw_lggr_clnt_lggr_tests
        PUBLIC
        os_cw_lggr_lggr)
target_link_libraries(
        os_cw_lggr_clnt_lggr_tests
        PUBLIC
        os_cw_lggr_clnt_lggr)
set_target_properties(
        os_cw_lggr_clnt_lggr_tests PROPERTIES
        LANGUAGES CXX
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
        CXX_EXTENSIONS OFF
        VERSION 1.0
        DESCRIPTION "client logger implementation library tests")