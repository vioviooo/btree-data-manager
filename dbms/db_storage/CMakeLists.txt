cmake_minimum_required(VERSION 3.21)
project(os_cw_dbms_db_strg)

add_subdirectory(server)

add_library(
        os_cw_dbms_db_strg
        src/db_storage.cpp)
target_include_directories(
        os_cw_dbms_db_strg
        PUBLIC
        ./include)
target_link_libraries(
        os_cw_dbms_db_strg
        PUBLIC
        os_cw_cmmn)
target_link_libraries(
        os_cw_dbms_db_strg
        PUBLIC
        os_cw_lggr_srvr_lggr)
target_link_libraries(
        os_cw_dbms_db_strg
        PUBLIC
        os_cw_assctv_cntnr)
target_link_libraries(
        os_cw_dbms_db_strg
        PUBLIC
        os_cw_assctv_cntnr_srch_tr)
target_link_libraries(
        os_cw_dbms_db_strg
        PUBLIC
        os_cw_assctv_cntnr_srch_tr_indxng_tr_b_tr)
target_link_libraries(
        os_cw_dbms_db_strg
        PUBLIC
        os_cw_allctr_allctr)
target_link_libraries(
        os_cw_dbms_db_strg
        PUBLIC
        os_cw_allctr_allctr_bndr_tgs)
target_link_libraries(
        os_cw_dbms_db_strg
        PUBLIC
        os_cw_allctr_allctr_glbl_hp)
target_link_libraries(
        os_cw_dbms_db_strg
        PUBLIC
        os_cw_allctr_allctr_buds_ssm)
target_link_libraries(
        os_cw_dbms_db_strg
        PUBLIC
        os_cw_allctr_allctr_std_lst)
target_link_libraries(
        os_cw_dbms_db_strg
        PUBLIC
        os_cw_allctr_allctr_rbt)
target_link_libraries(
        os_cw_dbms_db_strg
        PUBLIC
        os_cw_dbms_cmmn_types)
set_target_properties(
        os_cw_dbms_db_strg PROPERTIES
        LANGUAGES CXX
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
        CXX_EXTENSIONS OFF
        VERSION 1.0
        DESCRIPTION "database storage library")