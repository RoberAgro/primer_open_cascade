# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.14

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
CMAKE_COMMAND = /home/robertoa/my_software/jet_brains/clion/bin/cmake/linux/bin/cmake

# The command to remove a file.
RM = /home/robertoa/my_software/jet_brains/clion/bin/cmake/linux/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/robertoa/my_GIT/primer_open_cascade/open_cascade_demos/demo_bezier_surface

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/robertoa/my_GIT/primer_open_cascade/open_cascade_demos/demo_bezier_surface/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/demo_bezier_surface.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/demo_bezier_surface.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/demo_bezier_surface.dir/flags.make

CMakeFiles/demo_bezier_surface.dir/main.cpp.o: CMakeFiles/demo_bezier_surface.dir/flags.make
CMakeFiles/demo_bezier_surface.dir/main.cpp.o: ../main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/robertoa/my_GIT/primer_open_cascade/open_cascade_demos/demo_bezier_surface/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/demo_bezier_surface.dir/main.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/demo_bezier_surface.dir/main.cpp.o -c /home/robertoa/my_GIT/primer_open_cascade/open_cascade_demos/demo_bezier_surface/main.cpp

CMakeFiles/demo_bezier_surface.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/demo_bezier_surface.dir/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/robertoa/my_GIT/primer_open_cascade/open_cascade_demos/demo_bezier_surface/main.cpp > CMakeFiles/demo_bezier_surface.dir/main.cpp.i

CMakeFiles/demo_bezier_surface.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/demo_bezier_surface.dir/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/robertoa/my_GIT/primer_open_cascade/open_cascade_demos/demo_bezier_surface/main.cpp -o CMakeFiles/demo_bezier_surface.dir/main.cpp.s

# Object files for target demo_bezier_surface
demo_bezier_surface_OBJECTS = \
"CMakeFiles/demo_bezier_surface.dir/main.cpp.o"

# External object files for target demo_bezier_surface
demo_bezier_surface_EXTERNAL_OBJECTS =

demo_bezier_surface: CMakeFiles/demo_bezier_surface.dir/main.cpp.o
demo_bezier_surface: CMakeFiles/demo_bezier_surface.dir/build.make
demo_bezier_surface: CMakeFiles/demo_bezier_surface.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/robertoa/my_GIT/primer_open_cascade/open_cascade_demos/demo_bezier_surface/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable demo_bezier_surface"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/demo_bezier_surface.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/demo_bezier_surface.dir/build: demo_bezier_surface

.PHONY : CMakeFiles/demo_bezier_surface.dir/build

CMakeFiles/demo_bezier_surface.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/demo_bezier_surface.dir/cmake_clean.cmake
.PHONY : CMakeFiles/demo_bezier_surface.dir/clean

CMakeFiles/demo_bezier_surface.dir/depend:
	cd /home/robertoa/my_GIT/primer_open_cascade/open_cascade_demos/demo_bezier_surface/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/robertoa/my_GIT/primer_open_cascade/open_cascade_demos/demo_bezier_surface /home/robertoa/my_GIT/primer_open_cascade/open_cascade_demos/demo_bezier_surface /home/robertoa/my_GIT/primer_open_cascade/open_cascade_demos/demo_bezier_surface/cmake-build-debug /home/robertoa/my_GIT/primer_open_cascade/open_cascade_demos/demo_bezier_surface/cmake-build-debug /home/robertoa/my_GIT/primer_open_cascade/open_cascade_demos/demo_bezier_surface/cmake-build-debug/CMakeFiles/demo_bezier_surface.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/demo_bezier_surface.dir/depend

