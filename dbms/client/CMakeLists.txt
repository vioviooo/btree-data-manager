cmake_minimum_required(VERSION 3.21)
project(os_cw_dbms_clnt)

add_executable(
        os_cw_dbms_clnt
        client.cpp)
target_link_libraries(
        os_cw_dbms_clnt
        PUBLIC
        os_cw_dbms_cmmn_types)
set_target_properties(
        os_cw_dbms_clnt PROPERTIES
        LANGUAGES CXX
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
        CXX_EXTENSIONS OFF
        VERSION 1.0
        DESCRIPTION "just a client")