cmake_minimum_required(VERSION 3.21)
project(os_cw_lnx_lggng_srvr)

add_executable(
	os_cw_lnx_lggng_srvr
    src/linux_logging_server.cpp)
target_link_libraries(
        os_cw_lnx_lggng_srvr
        PUBLIC
        os_cw_cmmn)
target_link_libraries(
        os_cw_lnx_lggng_srvr
        PUBLIC
        os_cw_lggr_lggr)
target_link_libraries(
        os_cw_lnx_lggng_srvr
        PUBLIC
        os_cw_lggr_srvr_lggr)
set_target_properties(
        os_cw_lnx_lggng_srvr PROPERTIES
        LANGUAGES CXX
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
        CXX_EXTENSIONS OFF
        VERSION 1.0
        DESCRIPTION "linux logging server")