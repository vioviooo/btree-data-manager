cmake_minimum_required(VERSION 3.21)
project(os_cw_lggr_clnt_lggr)

add_subdirectory(tests)

include(FetchContent)
FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.11.2/json.tar.xz)
FetchContent_MakeAvailable(json)

add_library(
        os_cw_lggr_clnt_lggr
        src/client_logger.cpp
        src/client_logger_builder.cpp)
target_include_directories(
        os_cw_lggr_clnt_lggr
        PUBLIC
        ./include)
target_link_libraries(
        os_cw_lggr_clnt_lggr
        PUBLIC
        os_cw_cmmn)
target_link_libraries(
        os_cw_lggr_clnt_lggr
        PUBLIC
        os_cw_lggr_lggr)
target_link_libraries(
        os_cw_lggr_clnt_lggr
        PUBLIC
        nlohmann_json::nlohmann_json)
set_target_properties(
        os_cw_lggr_clnt_lggr PROPERTIES
        LANGUAGES CXX
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
        CXX_EXTENSIONS OFF
        VERSION 1.0
        DESCRIPTION "client logger implementation library")