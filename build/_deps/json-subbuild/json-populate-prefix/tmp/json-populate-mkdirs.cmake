# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/Users/vioviooo/Downloads/Telegram Desktop/os_cw_final 2/build/_deps/json-src"
  "/Users/vioviooo/Downloads/Telegram Desktop/os_cw_final 2/build/_deps/json-build"
  "/Users/vioviooo/Downloads/Telegram Desktop/os_cw_final 2/build/_deps/json-subbuild/json-populate-prefix"
  "/Users/vioviooo/Downloads/Telegram Desktop/os_cw_final 2/build/_deps/json-subbuild/json-populate-prefix/tmp"
  "/Users/vioviooo/Downloads/Telegram Desktop/os_cw_final 2/build/_deps/json-subbuild/json-populate-prefix/src/json-populate-stamp"
  "/Users/vioviooo/Downloads/Telegram Desktop/os_cw_final 2/build/_deps/json-subbuild/json-populate-prefix/src"
  "/Users/vioviooo/Downloads/Telegram Desktop/os_cw_final 2/build/_deps/json-subbuild/json-populate-prefix/src/json-populate-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/Users/vioviooo/Downloads/Telegram Desktop/os_cw_final 2/build/_deps/json-subbuild/json-populate-prefix/src/json-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/Users/vioviooo/Downloads/Telegram Desktop/os_cw_final 2/build/_deps/json-subbuild/json-populate-prefix/src/json-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()
