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
include CMakeFiles/run.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/run.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/run.dir/flags.make

CMakeFiles/run.dir/run.o: CMakeFiles/run.dir/flags.make
CMakeFiles/run.dir/run.o: run.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/karl/src/care/projects/xloem-intellect/starts/push/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/run.dir/run.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/run.dir/run.o -c /home/karl/src/care/projects/xloem-intellect/starts/push/run.cpp

CMakeFiles/run.dir/run.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/run.dir/run.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/karl/src/care/projects/xloem-intellect/starts/push/run.cpp > CMakeFiles/run.dir/run.i

CMakeFiles/run.dir/run.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/run.dir/run.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/karl/src/care/projects/xloem-intellect/starts/push/run.cpp -o CMakeFiles/run.dir/run.s

CMakeFiles/run.dir/run.o.requires:

.PHONY : CMakeFiles/run.dir/run.o.requires

CMakeFiles/run.dir/run.o.provides: CMakeFiles/run.dir/run.o.requires
	$(MAKE) -f CMakeFiles/run.dir/build.make CMakeFiles/run.dir/run.o.provides.build
.PHONY : CMakeFiles/run.dir/run.o.provides

CMakeFiles/run.dir/run.o.provides.build: CMakeFiles/run.dir/run.o


# Object files for target run
run_OBJECTS = \
"CMakeFiles/run.dir/run.o"

# External object files for target run
run_EXTERNAL_OBJECTS =

librun.so: CMakeFiles/run.dir/run.o
librun.so: CMakeFiles/run.dir/build.make
librun.so: CMakeFiles/run.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/karl/src/care/projects/xloem-intellect/starts/push/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX shared library librun.so"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/run.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/run.dir/build: librun.so

.PHONY : CMakeFiles/run.dir/build

CMakeFiles/run.dir/requires: CMakeFiles/run.dir/run.o.requires

.PHONY : CMakeFiles/run.dir/requires

CMakeFiles/run.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/run.dir/cmake_clean.cmake
.PHONY : CMakeFiles/run.dir/clean

CMakeFiles/run.dir/depend:
	cd /home/karl/src/care/projects/xloem-intellect/starts/push && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/karl/src/care/projects/xloem-intellect/starts/push /home/karl/src/care/projects/xloem-intellect/starts/push /home/karl/src/care/projects/xloem-intellect/starts/push /home/karl/src/care/projects/xloem-intellect/starts/push /home/karl/src/care/projects/xloem-intellect/starts/push/CMakeFiles/run.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/run.dir/depend

