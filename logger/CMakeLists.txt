cmake_minimum_required(VERSION 3.21)
project(os_cw_lggr
        VERSION 1.0
        DESCRIPTION "logger libraries"
        LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(logger)
add_subdirectory(client_logger)
add_subdirectory(server_logger)
add_subdirectory(linux_logging_server)