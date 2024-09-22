cmake_minimum_required(VERSION 3.21)
project(os_cw_assctv_cntnr_srch_tr_indxng_tr_b_tr)

#add_subdirectory(tests)
add_library(
        os_cw_assctv_cntnr_srch_tr_indxng_tr_b_tr
        SHARED
        include/b_tree.h)
target_include_directories(
        os_cw_assctv_cntnr_srch_tr_indxng_tr_b_tr
        PUBLIC
        ./include)
target_link_libraries(
        os_cw_assctv_cntnr_srch_tr_indxng_tr_b_tr
        PUBLIC
        os_cw_cmmn)
target_link_libraries(
        os_cw_assctv_cntnr_srch_tr_indxng_tr_b_tr
        PUBLIC
        os_cw_lggr_clnt_lggr)
target_link_libraries(
        os_cw_assctv_cntnr_srch_tr_indxng_tr_b_tr
        PUBLIC
        os_cw_assctv_cntnr)
target_link_libraries(
        os_cw_assctv_cntnr_srch_tr_indxng_tr_b_tr
        PUBLIC
        os_cw_assctv_cntnr_srch_tr)
set_target_properties(
        os_cw_assctv_cntnr_srch_tr_indxng_tr_b_tr PROPERTIES
        LANGUAGES CXX
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
        CXX_EXTENSIONS OFF
        VERSION 1.0
        DESCRIPTION "B tree implementation library")