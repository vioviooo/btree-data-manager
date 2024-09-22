cmake_minimum_required(VERSION 3.21)
project(os_cw_assctv_cntnr)

add_subdirectory(search_tree)
add_library(
        os_cw_assctv_cntnr
        SHARED
        include/associative_container.h)
target_include_directories(
        os_cw_assctv_cntnr
        PUBLIC
        ./include)
target_link_libraries(
        os_cw_assctv_cntnr
        PUBLIC
        os_cw_cmmn)
set_target_properties(
        os_cw_assctv_cntnr PROPERTIES
        VERSION 1.0
        DESCRIPTION "associative container interface library"
        LANGUAGES CXX
        LINKER_LANGUAGE CXX
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
        CXX_EXTENSIONS OFF)