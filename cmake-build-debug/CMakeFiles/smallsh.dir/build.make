# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.15

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = "/Users/jordanhamilton/Library/Application Support/JetBrains/Toolbox/apps/CLion/ch-0/192.7142.39/CLion.app/Contents/bin/cmake/mac/bin/cmake"

# The command to remove a file.
RM = "/Users/jordanhamilton/Library/Application Support/JetBrains/Toolbox/apps/CLion/ch-0/192.7142.39/CLion.app/Contents/bin/cmake/mac/bin/cmake" -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = "/Users/jordanhamilton/Documents/School/CS 344/Program 3"

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = "/Users/jordanhamilton/Documents/School/CS 344/Program 3/cmake-build-debug"

# Include any dependencies generated for this target.
include CMakeFiles/smallsh.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/smallsh.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/smallsh.dir/flags.make

CMakeFiles/smallsh.dir/smallsh.c.o: CMakeFiles/smallsh.dir/flags.make
CMakeFiles/smallsh.dir/smallsh.c.o: ../smallsh.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/Users/jordanhamilton/Documents/School/CS 344/Program 3/cmake-build-debug/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/smallsh.dir/smallsh.c.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/smallsh.dir/smallsh.c.o   -c "/Users/jordanhamilton/Documents/School/CS 344/Program 3/smallsh.c"

CMakeFiles/smallsh.dir/smallsh.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/smallsh.dir/smallsh.c.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E "/Users/jordanhamilton/Documents/School/CS 344/Program 3/smallsh.c" > CMakeFiles/smallsh.dir/smallsh.c.i

CMakeFiles/smallsh.dir/smallsh.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/smallsh.dir/smallsh.c.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S "/Users/jordanhamilton/Documents/School/CS 344/Program 3/smallsh.c" -o CMakeFiles/smallsh.dir/smallsh.c.s

# Object files for target smallsh
smallsh_OBJECTS = \
"CMakeFiles/smallsh.dir/smallsh.c.o"

# External object files for target smallsh
smallsh_EXTERNAL_OBJECTS =

smallsh: CMakeFiles/smallsh.dir/smallsh.c.o
smallsh: CMakeFiles/smallsh.dir/build.make
smallsh: CMakeFiles/smallsh.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir="/Users/jordanhamilton/Documents/School/CS 344/Program 3/cmake-build-debug/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable smallsh"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/smallsh.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/smallsh.dir/build: smallsh

.PHONY : CMakeFiles/smallsh.dir/build

CMakeFiles/smallsh.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/smallsh.dir/cmake_clean.cmake
.PHONY : CMakeFiles/smallsh.dir/clean

CMakeFiles/smallsh.dir/depend:
	cd "/Users/jordanhamilton/Documents/School/CS 344/Program 3/cmake-build-debug" && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" "/Users/jordanhamilton/Documents/School/CS 344/Program 3" "/Users/jordanhamilton/Documents/School/CS 344/Program 3" "/Users/jordanhamilton/Documents/School/CS 344/Program 3/cmake-build-debug" "/Users/jordanhamilton/Documents/School/CS 344/Program 3/cmake-build-debug" "/Users/jordanhamilton/Documents/School/CS 344/Program 3/cmake-build-debug/CMakeFiles/smallsh.dir/DependInfo.cmake" --color=$(COLOR)
.PHONY : CMakeFiles/smallsh.dir/depend

