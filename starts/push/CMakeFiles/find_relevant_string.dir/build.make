# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.10

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
CMAKE_SOURCE_DIR = /home/karl/src/care/projects/xloem-intellect/starts/push

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/karl/src/care/projects/xloem-intellect/starts/push

# Include any dependencies generated for this target.
include CMakeFiles/find_relevant_string.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/find_relevant_string.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/find_relevant_string.dir/flags.make

CMakeFiles/find_relevant_string.dir/find_relevant_string.o: CMakeFiles/find_relevant_string.dir/flags.make
CMakeFiles/find_relevant_string.dir/find_relevant_string.o: find_relevant_string.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/karl/src/care/projects/xloem-intellect/starts/push/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/find_relevant_string.dir/find_relevant_string.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/find_relevant_string.dir/find_relevant_string.o -c /home/karl/src/care/projects/xloem-intellect/starts/push/find_relevant_string.cpp

CMakeFiles/find_relevant_string.dir/find_relevant_string.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/find_relevant_string.dir/find_relevant_string.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/karl/src/care/projects/xloem-intellect/starts/push/find_relevant_string.cpp > CMakeFiles/find_relevant_string.dir/find_relevant_string.i

CMakeFiles/find_relevant_string.dir/find_relevant_string.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/find_relevant_string.dir/find_relevant_string.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/karl/src/care/projects/xloem-intellect/starts/push/find_relevant_string.cpp -o CMakeFiles/find_relevant_string.dir/find_relevant_string.s

CMakeFiles/find_relevant_string.dir/find_relevant_string.o.requires:

.PHONY : CMakeFiles/find_relevant_string.dir/find_relevant_string.o.requires

CMakeFiles/find_relevant_string.dir/find_relevant_string.o.provides: CMakeFiles/find_relevant_string.dir/find_relevant_string.o.requires
	$(MAKE) -f CMakeFiles/find_relevant_string.dir/build.make CMakeFiles/find_relevant_string.dir/find_relevant_string.o.provides.build
.PHONY : CMakeFiles/find_relevant_string.dir/find_relevant_string.o.provides

CMakeFiles/find_relevant_string.dir/find_relevant_string.o.provides.build: CMakeFiles/find_relevant_string.dir/find_relevant_string.o


# Object files for target find_relevant_string
find_relevant_string_OBJECTS = \
"CMakeFiles/find_relevant_string.dir/find_relevant_string.o"

# External object files for target find_relevant_string
find_relevant_string_EXTERNAL_OBJECTS =

libfind_relevant_string.so: CMakeFiles/find_relevant_string.dir/find_relevant_string.o
libfind_relevant_string.so: CMakeFiles/find_relevant_string.dir/build.make
libfind_relevant_string.so: CMakeFiles/find_relevant_string.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/karl/src/care/projects/xloem-intellect/starts/push/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX shared library libfind_relevant_string.so"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/find_relevant_string.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/find_relevant_string.dir/build: libfind_relevant_string.so

.PHONY : CMakeFiles/find_relevant_string.dir/build

CMakeFiles/find_relevant_string.dir/requires: CMakeFiles/find_relevant_string.dir/find_relevant_string.o.requires

.PHONY : CMakeFiles/find_relevant_string.dir/requires

CMakeFiles/find_relevant_string.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/find_relevant_string.dir/cmake_clean.cmake
.PHONY : CMakeFiles/find_relevant_string.dir/clean

CMakeFiles/find_relevant_string.dir/depend:
	cd /home/karl/src/care/projects/xloem-intellect/starts/push && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/karl/src/care/projects/xloem-intellect/starts/push /home/karl/src/care/projects/xloem-intellect/starts/push /home/karl/src/care/projects/xloem-intellect/starts/push /home/karl/src/care/projects/xloem-intellect/starts/push /home/karl/src/care/projects/xloem-intellect/starts/push/CMakeFiles/find_relevant_string.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/find_relevant_string.dir/depend
