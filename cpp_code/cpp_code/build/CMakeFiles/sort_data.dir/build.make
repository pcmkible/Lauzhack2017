# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.9

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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/aleksa/Lauzhack2017/cpp_code

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/aleksa/Lauzhack2017/cpp_code/build

# Include any dependencies generated for this target.
include CMakeFiles/sort_data.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/sort_data.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/sort_data.dir/flags.make

CMakeFiles/sort_data.dir/sort_data.cpp.o: CMakeFiles/sort_data.dir/flags.make
CMakeFiles/sort_data.dir/sort_data.cpp.o: ../sort_data.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/aleksa/Lauzhack2017/cpp_code/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/sort_data.dir/sort_data.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/sort_data.dir/sort_data.cpp.o -c /home/aleksa/Lauzhack2017/cpp_code/sort_data.cpp

CMakeFiles/sort_data.dir/sort_data.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/sort_data.dir/sort_data.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/aleksa/Lauzhack2017/cpp_code/sort_data.cpp > CMakeFiles/sort_data.dir/sort_data.cpp.i

CMakeFiles/sort_data.dir/sort_data.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/sort_data.dir/sort_data.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/aleksa/Lauzhack2017/cpp_code/sort_data.cpp -o CMakeFiles/sort_data.dir/sort_data.cpp.s

CMakeFiles/sort_data.dir/sort_data.cpp.o.requires:

.PHONY : CMakeFiles/sort_data.dir/sort_data.cpp.o.requires

CMakeFiles/sort_data.dir/sort_data.cpp.o.provides: CMakeFiles/sort_data.dir/sort_data.cpp.o.requires
	$(MAKE) -f CMakeFiles/sort_data.dir/build.make CMakeFiles/sort_data.dir/sort_data.cpp.o.provides.build
.PHONY : CMakeFiles/sort_data.dir/sort_data.cpp.o.provides

CMakeFiles/sort_data.dir/sort_data.cpp.o.provides.build: CMakeFiles/sort_data.dir/sort_data.cpp.o


# Object files for target sort_data
sort_data_OBJECTS = \
"CMakeFiles/sort_data.dir/sort_data.cpp.o"

# External object files for target sort_data
sort_data_EXTERNAL_OBJECTS =

sort_data: CMakeFiles/sort_data.dir/sort_data.cpp.o
sort_data: CMakeFiles/sort_data.dir/build.make
sort_data: /usr/lib/x86_64-linux-gnu/libboost_filesystem.so
sort_data: /usr/lib/x86_64-linux-gnu/libboost_system.so
sort_data: CMakeFiles/sort_data.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/aleksa/Lauzhack2017/cpp_code/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable sort_data"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/sort_data.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/sort_data.dir/build: sort_data

.PHONY : CMakeFiles/sort_data.dir/build

CMakeFiles/sort_data.dir/requires: CMakeFiles/sort_data.dir/sort_data.cpp.o.requires

.PHONY : CMakeFiles/sort_data.dir/requires

CMakeFiles/sort_data.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/sort_data.dir/cmake_clean.cmake
.PHONY : CMakeFiles/sort_data.dir/clean

CMakeFiles/sort_data.dir/depend:
	cd /home/aleksa/Lauzhack2017/cpp_code/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/aleksa/Lauzhack2017/cpp_code /home/aleksa/Lauzhack2017/cpp_code /home/aleksa/Lauzhack2017/cpp_code/build /home/aleksa/Lauzhack2017/cpp_code/build /home/aleksa/Lauzhack2017/cpp_code/build/CMakeFiles/sort_data.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/sort_data.dir/depend

