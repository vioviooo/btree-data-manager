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
include logger/logger/CMakeFiles/os_cw_lggr_lggr.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include logger/logger/CMakeFiles/os_cw_lggr_lggr.dir/compiler_depend.make

# Include the progress variables for this target.
include logger/logger/CMakeFiles/os_cw_lggr_lggr.dir/progress.make

# Include the compile flags for this target's objects.
include logger/logger/CMakeFiles/os_cw_lggr_lggr.dir/flags.make

logger/logger/CMakeFiles/os_cw_lggr_lggr.dir/src/logger.cpp.o: logger/logger/CMakeFiles/os_cw_lggr_lggr.dir/flags.make
logger/logger/CMakeFiles/os_cw_lggr_lggr.dir/src/logger.cpp.o: /Users/vioviooo/Downloads/Telegram\ Desktop/os_cw_final\ 2/logger/logger/src/logger.cpp
logger/logger/CMakeFiles/os_cw_lggr_lggr.dir/src/logger.cpp.o: logger/logger/CMakeFiles/os_cw_lggr_lggr.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir="/Users/vioviooo/Downloads/Telegram Desktop/os_cw_final 2/build/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object logger/logger/CMakeFiles/os_cw_lggr_lggr.dir/src/logger.cpp.o"
	cd "/Users/vioviooo/Downloads/Telegram Desktop/os_cw_final 2/build/logger/logger" && /usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT logger/logger/CMakeFiles/os_cw_lggr_lggr.dir/src/logger.cpp.o -MF CMakeFiles/os_cw_lggr_lggr.dir/src/logger.cpp.o.d -o CMakeFiles/os_cw_lggr_lggr.dir/src/logger.cpp.o -c "/Users/vioviooo/Downloads/Telegram Desktop/os_cw_final 2/logger/logger/src/logger.cpp"

logger/logger/CMakeFiles/os_cw_lggr_lggr.dir/src/logger.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/os_cw_lggr_lggr.dir/src/logger.cpp.i"
	cd "/Users/vioviooo/Downloads/Telegram Desktop/os_cw_final 2/build/logger/logger" && /usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/Users/vioviooo/Downloads/Telegram Desktop/os_cw_final 2/logger/logger/src/logger.cpp" > CMakeFiles/os_cw_lggr_lggr.dir/src/logger.cpp.i

logger/logger/CMakeFiles/os_cw_lggr_lggr.dir/src/logger.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/os_cw_lggr_lggr.dir/src/logger.cpp.s"
	cd "/Users/vioviooo/Downloads/Telegram Desktop/os_cw_final 2/build/logger/logger" && /usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/Users/vioviooo/Downloads/Telegram Desktop/os_cw_final 2/logger/logger/src/logger.cpp" -o CMakeFiles/os_cw_lggr_lggr.dir/src/logger.cpp.s

logger/logger/CMakeFiles/os_cw_lggr_lggr.dir/src/logger_builder.cpp.o: logger/logger/CMakeFiles/os_cw_lggr_lggr.dir/flags.make
logger/logger/CMakeFiles/os_cw_lggr_lggr.dir/src/logger_builder.cpp.o: /Users/vioviooo/Downloads/Telegram\ Desktop/os_cw_final\ 2/logger/logger/src/logger_builder.cpp
logger/logger/CMakeFiles/os_cw_lggr_lggr.dir/src/logger_builder.cpp.o: logger/logger/CMakeFiles/os_cw_lggr_lggr.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir="/Users/vioviooo/Downloads/Telegram Desktop/os_cw_final 2/build/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object logger/logger/CMakeFiles/os_cw_lggr_lggr.dir/src/logger_builder.cpp.o"
	cd "/Users/vioviooo/Downloads/Telegram Desktop/os_cw_final 2/build/logger/logger" && /usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT logger/logger/CMakeFiles/os_cw_lggr_lggr.dir/src/logger_builder.cpp.o -MF CMakeFiles/os_cw_lggr_lggr.dir/src/logger_builder.cpp.o.d -o CMakeFiles/os_cw_lggr_lggr.dir/src/logger_builder.cpp.o -c "/Users/vioviooo/Downloads/Telegram Desktop/os_cw_final 2/logger/logger/src/logger_builder.cpp"

logger/logger/CMakeFiles/os_cw_lggr_lggr.dir/src/logger_builder.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/os_cw_lggr_lggr.dir/src/logger_builder.cpp.i"
	cd "/Users/vioviooo/Downloads/Telegram Desktop/os_cw_final 2/build/logger/logger" && /usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/Users/vioviooo/Downloads/Telegram Desktop/os_cw_final 2/logger/logger/src/logger_builder.cpp" > CMakeFiles/os_cw_lggr_lggr.dir/src/logger_builder.cpp.i

logger/logger/CMakeFiles/os_cw_lggr_lggr.dir/src/logger_builder.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/os_cw_lggr_lggr.dir/src/logger_builder.cpp.s"
	cd "/Users/vioviooo/Downloads/Telegram Desktop/os_cw_final 2/build/logger/logger" && /usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/Users/vioviooo/Downloads/Telegram Desktop/os_cw_final 2/logger/logger/src/logger_builder.cpp" -o CMakeFiles/os_cw_lggr_lggr.dir/src/logger_builder.cpp.s

logger/logger/CMakeFiles/os_cw_lggr_lggr.dir/src/logger_guardant.cpp.o: logger/logger/CMakeFiles/os_cw_lggr_lggr.dir/flags.make
logger/logger/CMakeFiles/os_cw_lggr_lggr.dir/src/logger_guardant.cpp.o: /Users/vioviooo/Downloads/Telegram\ Desktop/os_cw_final\ 2/logger/logger/src/logger_guardant.cpp
logger/logger/CMakeFiles/os_cw_lggr_lggr.dir/src/logger_guardant.cpp.o: logger/logger/CMakeFiles/os_cw_lggr_lggr.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir="/Users/vioviooo/Downloads/Telegram Desktop/os_cw_final 2/build/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object logger/logger/CMakeFiles/os_cw_lggr_lggr.dir/src/logger_guardant.cpp.o"
	cd "/Users/vioviooo/Downloads/Telegram Desktop/os_cw_final 2/build/logger/logger" && /usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT logger/logger/CMakeFiles/os_cw_lggr_lggr.dir/src/logger_guardant.cpp.o -MF CMakeFiles/os_cw_lggr_lggr.dir/src/logger_guardant.cpp.o.d -o CMakeFiles/os_cw_lggr_lggr.dir/src/logger_guardant.cpp.o -c "/Users/vioviooo/Downloads/Telegram Desktop/os_cw_final 2/logger/logger/src/logger_guardant.cpp"

logger/logger/CMakeFiles/os_cw_lggr_lggr.dir/src/logger_guardant.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/os_cw_lggr_lggr.dir/src/logger_guardant.cpp.i"
	cd "/Users/vioviooo/Downloads/Telegram Desktop/os_cw_final 2/build/logger/logger" && /usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/Users/vioviooo/Downloads/Telegram Desktop/os_cw_final 2/logger/logger/src/logger_guardant.cpp" > CMakeFiles/os_cw_lggr_lggr.dir/src/logger_guardant.cpp.i

logger/logger/CMakeFiles/os_cw_lggr_lggr.dir/src/logger_guardant.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/os_cw_lggr_lggr.dir/src/logger_guardant.cpp.s"
	cd "/Users/vioviooo/Downloads/Telegram Desktop/os_cw_final 2/build/logger/logger" && /usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/Users/vioviooo/Downloads/Telegram Desktop/os_cw_final 2/logger/logger/src/logger_guardant.cpp" -o CMakeFiles/os_cw_lggr_lggr.dir/src/logger_guardant.cpp.s

# Object files for target os_cw_lggr_lggr
os_cw_lggr_lggr_OBJECTS = \
"CMakeFiles/os_cw_lggr_lggr.dir/src/logger.cpp.o" \
"CMakeFiles/os_cw_lggr_lggr.dir/src/logger_builder.cpp.o" \
"CMakeFiles/os_cw_lggr_lggr.dir/src/logger_guardant.cpp.o"

# External object files for target os_cw_lggr_lggr
os_cw_lggr_lggr_EXTERNAL_OBJECTS =

logger/logger/libos_cw_lggr_lggr.a: logger/logger/CMakeFiles/os_cw_lggr_lggr.dir/src/logger.cpp.o
logger/logger/libos_cw_lggr_lggr.a: logger/logger/CMakeFiles/os_cw_lggr_lggr.dir/src/logger_builder.cpp.o
logger/logger/libos_cw_lggr_lggr.a: logger/logger/CMakeFiles/os_cw_lggr_lggr.dir/src/logger_guardant.cpp.o
logger/logger/libos_cw_lggr_lggr.a: logger/logger/CMakeFiles/os_cw_lggr_lggr.dir/build.make
logger/logger/libos_cw_lggr_lggr.a: logger/logger/CMakeFiles/os_cw_lggr_lggr.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir="/Users/vioviooo/Downloads/Telegram Desktop/os_cw_final 2/build/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_4) "Linking CXX static library libos_cw_lggr_lggr.a"
	cd "/Users/vioviooo/Downloads/Telegram Desktop/os_cw_final 2/build/logger/logger" && $(CMAKE_COMMAND) -P CMakeFiles/os_cw_lggr_lggr.dir/cmake_clean_target.cmake
	cd "/Users/vioviooo/Downloads/Telegram Desktop/os_cw_final 2/build/logger/logger" && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/os_cw_lggr_lggr.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
logger/logger/CMakeFiles/os_cw_lggr_lggr.dir/build: logger/logger/libos_cw_lggr_lggr.a
.PHONY : logger/logger/CMakeFiles/os_cw_lggr_lggr.dir/build

logger/logger/CMakeFiles/os_cw_lggr_lggr.dir/clean:
	cd "/Users/vioviooo/Downloads/Telegram Desktop/os_cw_final 2/build/logger/logger" && $(CMAKE_COMMAND) -P CMakeFiles/os_cw_lggr_lggr.dir/cmake_clean.cmake
.PHONY : logger/logger/CMakeFiles/os_cw_lggr_lggr.dir/clean

logger/logger/CMakeFiles/os_cw_lggr_lggr.dir/depend:
	cd "/Users/vioviooo/Downloads/Telegram Desktop/os_cw_final 2/build" && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" "/Users/vioviooo/Downloads/Telegram Desktop/os_cw_final 2" "/Users/vioviooo/Downloads/Telegram Desktop/os_cw_final 2/logger/logger" "/Users/vioviooo/Downloads/Telegram Desktop/os_cw_final 2/build" "/Users/vioviooo/Downloads/Telegram Desktop/os_cw_final 2/build/logger/logger" "/Users/vioviooo/Downloads/Telegram Desktop/os_cw_final 2/build/logger/logger/CMakeFiles/os_cw_lggr_lggr.dir/DependInfo.cmake" "--color=$(COLOR)"
.PHONY : logger/logger/CMakeFiles/os_cw_lggr_lggr.dir/depend

