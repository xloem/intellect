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
include CMakeFiles/copy_data.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/copy_data.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/copy_data.dir/flags.make

CMakeFiles/copy_data.dir/copy_data.o: CMakeFiles/copy_data.dir/flags.make
CMakeFiles/copy_data.dir/copy_data.o: copy_data.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/karl/src/care/projects/xloem-intellect/starts/push/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/copy_data.dir/copy_data.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/copy_data.dir/copy_data.o -c /home/karl/src/care/projects/xloem-intellect/starts/push/copy_data.cpp

CMakeFiles/copy_data.dir/copy_data.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/copy_data.dir/copy_data.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/karl/src/care/projects/xloem-intellect/starts/push/copy_data.cpp > CMakeFiles/copy_data.dir/copy_data.i

CMakeFiles/copy_data.dir/copy_data.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/copy_data.dir/copy_data.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/karl/src/care/projects/xloem-intellect/starts/push/copy_data.cpp -o CMakeFiles/copy_data.dir/copy_data.s

CMakeFiles/copy_data.dir/copy_data.o.requires:

.PHONY : CMakeFiles/copy_data.dir/copy_data.o.requires

CMakeFiles/copy_data.dir/copy_data.o.provides: CMakeFiles/copy_data.dir/copy_data.o.requires
	$(MAKE) -f CMakeFiles/copy_data.dir/build.make CMakeFiles/copy_data.dir/copy_data.o.provides.build
.PHONY : CMakeFiles/copy_data.dir/copy_data.o.provides

CMakeFiles/copy_data.dir/copy_data.o.provides.build: CMakeFiles/copy_data.dir/copy_data.o


# Object files for target copy_data
copy_data_OBJECTS = \
"CMakeFiles/copy_data.dir/copy_data.o"

# External object files for target copy_data
copy_data_EXTERNAL_OBJECTS =

libcopy_data.so: CMakeFiles/copy_data.dir/copy_data.o
libcopy_data.so: CMakeFiles/copy_data.dir/build.make
libcopy_data.so: CMakeFiles/copy_data.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/karl/src/care/projects/xloem-intellect/starts/push/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX shared library libcopy_data.so"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/copy_data.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/copy_data.dir/build: libcopy_data.so

.PHONY : CMakeFiles/copy_data.dir/build

CMakeFiles/copy_data.dir/requires: CMakeFiles/copy_data.dir/copy_data.o.requires

.PHONY : CMakeFiles/copy_data.dir/requires

CMakeFiles/copy_data.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/copy_data.dir/cmake_clean.cmake
.PHONY : CMakeFiles/copy_data.dir/clean

CMakeFiles/copy_data.dir/depend:
	cd /home/karl/src/care/projects/xloem-intellect/starts/push && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/karl/src/care/projects/xloem-intellect/starts/push /home/karl/src/care/projects/xloem-intellect/starts/push /home/karl/src/care/projects/xloem-intellect/starts/push /home/karl/src/care/projects/xloem-intellect/starts/push /home/karl/src/care/projects/xloem-intellect/starts/push/CMakeFiles/copy_data.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/copy_data.dir/depend

