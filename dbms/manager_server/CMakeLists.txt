cmake_minimum_required(VERSION 3.21)
project(os_cw_dbms_mngr_srvr)

add_executable(
        os_cw_dbms_mngr_srvr
        manager_server.cpp)
target_link_libraries(
        os_cw_dbms_mngr_srvr
        PUBLIC
        os_cw_cmmn)
target_link_libraries(
        os_cw_dbms_mngr_srvr
        PUBLIC
        os_cw_dbms_cmmn_types)

target_link_libraries(
        os_cw_dbms_mngr_srvr
        PUBLIC
        os_cw_assctv_cntnr)
target_link_libraries(
        os_cw_dbms_mngr_srvr
        PUBLIC
        os_cw_assctv_cntnr_srch_tr)
target_link_libraries(
        os_cw_dbms_mngr_srvr
        PUBLIC
        os_cw_assctv_cntnr_srch_tr_indxng_tr_b_tr)

set_target_properties(
        os_cw_dbms_mngr_srvr PROPERTIES
        LANGUAGES CXX
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
        CXX_EXTENSIONS OFF
        VERSION 1.0
        DESCRIPTION "just a manager server")