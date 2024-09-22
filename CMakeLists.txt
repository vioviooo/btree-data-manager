cmake_minimum_required(VERSION 3.21)
project(os_cw)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(allocator)
add_subdirectory(associative_container)
add_subdirectory(common)
add_subdirectory(logger)
add_subdirectory(dbms)