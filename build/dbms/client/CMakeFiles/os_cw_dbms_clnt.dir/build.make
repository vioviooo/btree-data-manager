# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.28

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /opt/homebrew/Cellar/cmake/3.28.3/bin/cmake

# The command to remove a file.
RM = /opt/homebrew/Cellar/cmake/3.28.3/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = "/Users/vioviooo/Downloads/Telegram Desktop/os_cw_final 2"

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = "/Users/vioviooo/Downloads/Telegram Desktop/os_cw_final 2/build"

# Include any dependencies generated for this target.
include dbms/client/CMakeFiles/os_cw_dbms_clnt.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include dbms/client/CMakeFiles/os_cw_dbms_clnt.dir/compiler_depend.make

# Include the progress variables for this target.
include dbms/client/CMakeFiles/os_cw_dbms_clnt.dir/progress.make

# Include the compile flags for this target's objects.
include dbms/client/CMakeFiles/os_cw_dbms_clnt.dir/flags.make

dbms/client/CMakeFiles/os_cw_dbms_clnt.dir/client.cpp.o: dbms/client/CMakeFiles/os_cw_dbms_clnt.dir/flags.make
dbms/client/CMakeFiles/os_cw_dbms_clnt.dir/client.cpp.o: /Users/vioviooo/Downloads/Telegram\ Desktop/os_cw_final\ 2/dbms/client/client.cpp
dbms/client/CMakeFiles/os_cw_dbms_clnt.dir/client.cpp.o: dbms/client/CMakeFiles/os_cw_dbms_clnt.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir="/Users/vioviooo/Downloads/Telegram Desktop/os_cw_final 2/build/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object dbms/client/CMakeFiles/os_cw_dbms_clnt.dir/client.cpp.o"
	cd "/Users/vioviooo/Downloads/Telegram Desktop/os_cw_final 2/build/dbms/client" && /usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT dbms/client/CMakeFiles/os_cw_dbms_clnt.dir/client.cpp.o -MF CMakeFiles/os_cw_dbms_clnt.dir/client.cpp.o.d -o CMakeFiles/os_cw_dbms_clnt.dir/client.cpp.o -c "/Users/vioviooo/Downloads/Telegram Desktop/os_cw_final 2/dbms/client/client.cpp"

dbms/client/CMakeFiles/os_cw_dbms_clnt.dir/client.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/os_cw_dbms_clnt.dir/client.cpp.i"
	cd "/Users/vioviooo/Downloads/Telegram Desktop/os_cw_final 2/build/dbms/client" && /usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/Users/vioviooo/Downloads/Telegram Desktop/os_cw_final 2/dbms/client/client.cpp" > CMakeFiles/os_cw_dbms_clnt.dir/client.cpp.i

dbms/client/CMakeFiles/os_cw_dbms_clnt.dir/client.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/os_cw_dbms_clnt.dir/client.cpp.s"
	cd "/Users/vioviooo/Downloads/Telegram Desktop/os_cw_final 2/build/dbms/client" && /usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/Users/vioviooo/Downloads/Telegram Desktop/os_cw_final 2/dbms/client/client.cpp" -o CMakeFiles/os_cw_dbms_clnt.dir/client.cpp.s

# Object files for target os_cw_dbms_clnt
os_cw_dbms_clnt_OBJECTS = \
"CMakeFiles/os_cw_dbms_clnt.dir/client.cpp.o"

# External object files for target os_cw_dbms_clnt
os_cw_dbms_clnt_EXTERNAL_OBJECTS =

dbms/client/os_cw_dbms_clnt-1.0: dbms/client/CMakeFiles/os_cw_dbms_clnt.dir/client.cpp.o
dbms/client/os_cw_dbms_clnt-1.0: dbms/client/CMakeFiles/os_cw_dbms_clnt.dir/build.make
dbms/client/os_cw_dbms_clnt-1.0: dbms/common_types/libos_cw_dbms_cmmn_types.a
dbms/client/os_cw_dbms_clnt-1.0: common/libos_cw_cmmn.a
dbms/client/os_cw_dbms_clnt-1.0: allocator/allocator/libos_cw_allctr_allctr.a
dbms/client/os_cw_dbms_clnt-1.0: dbms/client/CMakeFiles/os_cw_dbms_clnt.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir="/Users/vioviooo/Downloads/Telegram Desktop/os_cw_final 2/build/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable os_cw_dbms_clnt"
	cd "/Users/vioviooo/Downloads/Telegram Desktop/os_cw_final 2/build/dbms/client" && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/os_cw_dbms_clnt.dir/link.txt --verbose=$(VERBOSE)
	cd "/Users/vioviooo/Downloads/Telegram Desktop/os_cw_final 2/build/dbms/client" && $(CMAKE_COMMAND) -E cmake_symlink_executable os_cw_dbms_clnt-1.0 os_cw_dbms_clnt

dbms/client/os_cw_dbms_clnt: dbms/client/os_cw_dbms_clnt-1.0

# Rule to build all files generated by this target.
dbms/client/CMakeFiles/os_cw_dbms_clnt.dir/build: dbms/client/os_cw_dbms_clnt
.PHONY : dbms/client/CMakeFiles/os_cw_dbms_clnt.dir/build

dbms/client/CMakeFiles/os_cw_dbms_clnt.dir/clean:
	cd "/Users/vioviooo/Downloads/Telegram Desktop/os_cw_final 2/build/dbms/client" && $(CMAKE_COMMAND) -P CMakeFiles/os_cw_dbms_clnt.dir/cmake_clean.cmake
.PHONY : dbms/client/CMakeFiles/os_cw_dbms_clnt.dir/clean

dbms/client/CMakeFiles/os_cw_dbms_clnt.dir/depend:
	cd "/Users/vioviooo/Downloads/Telegram Desktop/os_cw_final 2/build" && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" "/Users/vioviooo/Downloads/Telegram Desktop/os_cw_final 2" "/Users/vioviooo/Downloads/Telegram Desktop/os_cw_final 2/dbms/client" "/Users/vioviooo/Downloads/Telegram Desktop/os_cw_final 2/build" "/Users/vioviooo/Downloads/Telegram Desktop/os_cw_final 2/build/dbms/client" "/Users/vioviooo/Downloads/Telegram Desktop/os_cw_final 2/build/dbms/client/CMakeFiles/os_cw_dbms_clnt.dir/DependInfo.cmake" "--color=$(COLOR)"
.PHONY : dbms/client/CMakeFiles/os_cw_dbms_clnt.dir/depend

