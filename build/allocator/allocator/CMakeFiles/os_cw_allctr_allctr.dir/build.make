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
include allocator/allocator/CMakeFiles/os_cw_allctr_allctr.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include allocator/allocator/CMakeFiles/os_cw_allctr_allctr.dir/compiler_depend.make

# Include the progress variables for this target.
include allocator/allocator/CMakeFiles/os_cw_allctr_allctr.dir/progress.make

# Include the compile flags for this target's objects.
include allocator/allocator/CMakeFiles/os_cw_allctr_allctr.dir/flags.make

allocator/allocator/CMakeFiles/os_cw_allctr_allctr.dir/src/allocator.cpp.o: allocator/allocator/CMakeFiles/os_cw_allctr_allctr.dir/flags.make
allocator/allocator/CMakeFiles/os_cw_allctr_allctr.dir/src/allocator.cpp.o: /Users/vioviooo/Downloads/Telegram\ Desktop/os_cw_final\ 2/allocator/allocator/src/allocator.cpp
allocator/allocator/CMakeFiles/os_cw_allctr_allctr.dir/src/allocator.cpp.o: allocator/allocator/CMakeFiles/os_cw_allctr_allctr.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir="/Users/vioviooo/Downloads/Telegram Desktop/os_cw_final 2/build/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object allocator/allocator/CMakeFiles/os_cw_allctr_allctr.dir/src/allocator.cpp.o"
	cd "/Users/vioviooo/Downloads/Telegram Desktop/os_cw_final 2/build/allocator/allocator" && /usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT allocator/allocator/CMakeFiles/os_cw_allctr_allctr.dir/src/allocator.cpp.o -MF CMakeFiles/os_cw_allctr_allctr.dir/src/allocator.cpp.o.d -o CMakeFiles/os_cw_allctr_allctr.dir/src/allocator.cpp.o -c "/Users/vioviooo/Downloads/Telegram Desktop/os_cw_final 2/allocator/allocator/src/allocator.cpp"

allocator/allocator/CMakeFiles/os_cw_allctr_allctr.dir/src/allocator.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/os_cw_allctr_allctr.dir/src/allocator.cpp.i"
	cd "/Users/vioviooo/Downloads/Telegram Desktop/os_cw_final 2/build/allocator/allocator" && /usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/Users/vioviooo/Downloads/Telegram Desktop/os_cw_final 2/allocator/allocator/src/allocator.cpp" > CMakeFiles/os_cw_allctr_allctr.dir/src/allocator.cpp.i

allocator/allocator/CMakeFiles/os_cw_allctr_allctr.dir/src/allocator.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/os_cw_allctr_allctr.dir/src/allocator.cpp.s"
	cd "/Users/vioviooo/Downloads/Telegram Desktop/os_cw_final 2/build/allocator/allocator" && /usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/Users/vioviooo/Downloads/Telegram Desktop/os_cw_final 2/allocator/allocator/src/allocator.cpp" -o CMakeFiles/os_cw_allctr_allctr.dir/src/allocator.cpp.s

allocator/allocator/CMakeFiles/os_cw_allctr_allctr.dir/src/allocator_guardant.cpp.o: allocator/allocator/CMakeFiles/os_cw_allctr_allctr.dir/flags.make
allocator/allocator/CMakeFiles/os_cw_allctr_allctr.dir/src/allocator_guardant.cpp.o: /Users/vioviooo/Downloads/Telegram\ Desktop/os_cw_final\ 2/allocator/allocator/src/allocator_guardant.cpp
allocator/allocator/CMakeFiles/os_cw_allctr_allctr.dir/src/allocator_guardant.cpp.o: allocator/allocator/CMakeFiles/os_cw_allctr_allctr.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir="/Users/vioviooo/Downloads/Telegram Desktop/os_cw_final 2/build/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object allocator/allocator/CMakeFiles/os_cw_allctr_allctr.dir/src/allocator_guardant.cpp.o"
	cd "/Users/vioviooo/Downloads/Telegram Desktop/os_cw_final 2/build/allocator/allocator" && /usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT allocator/allocator/CMakeFiles/os_cw_allctr_allctr.dir/src/allocator_guardant.cpp.o -MF CMakeFiles/os_cw_allctr_allctr.dir/src/allocator_guardant.cpp.o.d -o CMakeFiles/os_cw_allctr_allctr.dir/src/allocator_guardant.cpp.o -c "/Users/vioviooo/Downloads/Telegram Desktop/os_cw_final 2/allocator/allocator/src/allocator_guardant.cpp"

allocator/allocator/CMakeFiles/os_cw_allctr_allctr.dir/src/allocator_guardant.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/os_cw_allctr_allctr.dir/src/allocator_guardant.cpp.i"
	cd "/Users/vioviooo/Downloads/Telegram Desktop/os_cw_final 2/build/allocator/allocator" && /usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/Users/vioviooo/Downloads/Telegram Desktop/os_cw_final 2/allocator/allocator/src/allocator_guardant.cpp" > CMakeFiles/os_cw_allctr_allctr.dir/src/allocator_guardant.cpp.i

allocator/allocator/CMakeFiles/os_cw_allctr_allctr.dir/src/allocator_guardant.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/os_cw_allctr_allctr.dir/src/allocator_guardant.cpp.s"
	cd "/Users/vioviooo/Downloads/Telegram Desktop/os_cw_final 2/build/allocator/allocator" && /usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/Users/vioviooo/Downloads/Telegram Desktop/os_cw_final 2/allocator/allocator/src/allocator_guardant.cpp" -o CMakeFiles/os_cw_allctr_allctr.dir/src/allocator_guardant.cpp.s

allocator/allocator/CMakeFiles/os_cw_allctr_allctr.dir/src/allocator_test_utils.cpp.o: allocator/allocator/CMakeFiles/os_cw_allctr_allctr.dir/flags.make
allocator/allocator/CMakeFiles/os_cw_allctr_allctr.dir/src/allocator_test_utils.cpp.o: /Users/vioviooo/Downloads/Telegram\ Desktop/os_cw_final\ 2/allocator/allocator/src/allocator_test_utils.cpp
allocator/allocator/CMakeFiles/os_cw_allctr_allctr.dir/src/allocator_test_utils.cpp.o: allocator/allocator/CMakeFiles/os_cw_allctr_allctr.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir="/Users/vioviooo/Downloads/Telegram Desktop/os_cw_final 2/build/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object allocator/allocator/CMakeFiles/os_cw_allctr_allctr.dir/src/allocator_test_utils.cpp.o"
	cd "/Users/vioviooo/Downloads/Telegram Desktop/os_cw_final 2/build/allocator/allocator" && /usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT allocator/allocator/CMakeFiles/os_cw_allctr_allctr.dir/src/allocator_test_utils.cpp.o -MF CMakeFiles/os_cw_allctr_allctr.dir/src/allocator_test_utils.cpp.o.d -o CMakeFiles/os_cw_allctr_allctr.dir/src/allocator_test_utils.cpp.o -c "/Users/vioviooo/Downloads/Telegram Desktop/os_cw_final 2/allocator/allocator/src/allocator_test_utils.cpp"

allocator/allocator/CMakeFiles/os_cw_allctr_allctr.dir/src/allocator_test_utils.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/os_cw_allctr_allctr.dir/src/allocator_test_utils.cpp.i"
	cd "/Users/vioviooo/Downloads/Telegram Desktop/os_cw_final 2/build/allocator/allocator" && /usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/Users/vioviooo/Downloads/Telegram Desktop/os_cw_final 2/allocator/allocator/src/allocator_test_utils.cpp" > CMakeFiles/os_cw_allctr_allctr.dir/src/allocator_test_utils.cpp.i

allocator/allocator/CMakeFiles/os_cw_allctr_allctr.dir/src/allocator_test_utils.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/os_cw_allctr_allctr.dir/src/allocator_test_utils.cpp.s"
	cd "/Users/vioviooo/Downloads/Telegram Desktop/os_cw_final 2/build/allocator/allocator" && /usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/Users/vioviooo/Downloads/Telegram Desktop/os_cw_final 2/allocator/allocator/src/allocator_test_utils.cpp" -o CMakeFiles/os_cw_allctr_allctr.dir/src/allocator_test_utils.cpp.s

# Object files for target os_cw_allctr_allctr
os_cw_allctr_allctr_OBJECTS = \
"CMakeFiles/os_cw_allctr_allctr.dir/src/allocator.cpp.o" \
"CMakeFiles/os_cw_allctr_allctr.dir/src/allocator_guardant.cpp.o" \
"CMakeFiles/os_cw_allctr_allctr.dir/src/allocator_test_utils.cpp.o"

# External object files for target os_cw_allctr_allctr
os_cw_allctr_allctr_EXTERNAL_OBJECTS =

allocator/allocator/libos_cw_allctr_allctr.a: allocator/allocator/CMakeFiles/os_cw_allctr_allctr.dir/src/allocator.cpp.o
allocator/allocator/libos_cw_allctr_allctr.a: allocator/allocator/CMakeFiles/os_cw_allctr_allctr.dir/src/allocator_guardant.cpp.o
allocator/allocator/libos_cw_allctr_allctr.a: allocator/allocator/CMakeFiles/os_cw_allctr_allctr.dir/src/allocator_test_utils.cpp.o
allocator/allocator/libos_cw_allctr_allctr.a: allocator/allocator/CMakeFiles/os_cw_allctr_allctr.dir/build.make
allocator/allocator/libos_cw_allctr_allctr.a: allocator/allocator/CMakeFiles/os_cw_allctr_allctr.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir="/Users/vioviooo/Downloads/Telegram Desktop/os_cw_final 2/build/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_4) "Linking CXX static library libos_cw_allctr_allctr.a"
	cd "/Users/vioviooo/Downloads/Telegram Desktop/os_cw_final 2/build/allocator/allocator" && $(CMAKE_COMMAND) -P CMakeFiles/os_cw_allctr_allctr.dir/cmake_clean_target.cmake
	cd "/Users/vioviooo/Downloads/Telegram Desktop/os_cw_final 2/build/allocator/allocator" && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/os_cw_allctr_allctr.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
allocator/allocator/CMakeFiles/os_cw_allctr_allctr.dir/build: allocator/allocator/libos_cw_allctr_allctr.a
.PHONY : allocator/allocator/CMakeFiles/os_cw_allctr_allctr.dir/build

allocator/allocator/CMakeFiles/os_cw_allctr_allctr.dir/clean:
	cd "/Users/vioviooo/Downloads/Telegram Desktop/os_cw_final 2/build/allocator/allocator" && $(CMAKE_COMMAND) -P CMakeFiles/os_cw_allctr_allctr.dir/cmake_clean.cmake
.PHONY : allocator/allocator/CMakeFiles/os_cw_allctr_allctr.dir/clean

allocator/allocator/CMakeFiles/os_cw_allctr_allctr.dir/depend:
	cd "/Users/vioviooo/Downloads/Telegram Desktop/os_cw_final 2/build" && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" "/Users/vioviooo/Downloads/Telegram Desktop/os_cw_final 2" "/Users/vioviooo/Downloads/Telegram Desktop/os_cw_final 2/allocator/allocator" "/Users/vioviooo/Downloads/Telegram Desktop/os_cw_final 2/build" "/Users/vioviooo/Downloads/Telegram Desktop/os_cw_final 2/build/allocator/allocator" "/Users/vioviooo/Downloads/Telegram Desktop/os_cw_final 2/build/allocator/allocator/CMakeFiles/os_cw_allctr_allctr.dir/DependInfo.cmake" "--color=$(COLOR)"
.PHONY : allocator/allocator/CMakeFiles/os_cw_allctr_allctr.dir/depend

