cmake_minimum_required(VERSION 3.21)
project(os_cw_assctv_cntnr_srch_tr)

add_subdirectory(indexing_tree)
add_library(
        os_cw_assctv_cntnr_srch_tr
        SHARED
        include/search_tree.h)
target_include_directories(
        os_cw_assctv_cntnr_srch_tr
        PUBLIC
        ./include)
target_link_libraries(
        os_cw_assctv_cntnr_srch_tr
        PUBLIC
        os_cw_cmmn)
target_link_libraries(
        os_cw_assctv_cntnr_srch_tr
        PUBLIC
        os_cw_lggr_lggr)
target_link_libraries(
        os_cw_assctv_cntnr_srch_tr
        PUBLIC
        os_cw_allctr_allctr)
target_link_libraries(
        os_cw_assctv_cntnr_srch_tr
        PUBLIC
        os_cw_assctv_cntnr)
set_target_properties(
        os_cw_assctv_cntnr_srch_tr PROPERTIES
        VERSION 1.0
        DESCRIPTION "search tree interface library"
        LANGUAGES CXX
        LINKER_LANGUAGE CXX
        LINKER_LANGUAGE CXX
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
        CXX_EXTENSIONS OFF)