# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

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
CMAKE_SOURCE_DIR = /home/morell/projects/HSE-Course-Private/module-1/lectures/lecture19/type_traits

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/morell/projects/HSE-Course-Private/module-1/lectures/lecture19/type_traits/build

# Include any dependencies generated for this target.
include CMakeFiles/runner.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/runner.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/runner.dir/flags.make

CMakeFiles/runner.dir/0_spec.cpp.o: CMakeFiles/runner.dir/flags.make
CMakeFiles/runner.dir/0_spec.cpp.o: ../0_spec.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/morell/projects/HSE-Course-Private/module-1/lectures/lecture19/type_traits/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/runner.dir/0_spec.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/runner.dir/0_spec.cpp.o -c /home/morell/projects/HSE-Course-Private/module-1/lectures/lecture19/type_traits/0_spec.cpp

CMakeFiles/runner.dir/0_spec.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/runner.dir/0_spec.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/morell/projects/HSE-Course-Private/module-1/lectures/lecture19/type_traits/0_spec.cpp > CMakeFiles/runner.dir/0_spec.cpp.i

CMakeFiles/runner.dir/0_spec.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/runner.dir/0_spec.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/morell/projects/HSE-Course-Private/module-1/lectures/lecture19/type_traits/0_spec.cpp -o CMakeFiles/runner.dir/0_spec.cpp.s

# Object files for target runner
runner_OBJECTS = \
"CMakeFiles/runner.dir/0_spec.cpp.o"

# External object files for target runner
runner_EXTERNAL_OBJECTS =

runner: CMakeFiles/runner.dir/0_spec.cpp.o
runner: CMakeFiles/runner.dir/build.make
runner: CMakeFiles/runner.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/morell/projects/HSE-Course-Private/module-1/lectures/lecture19/type_traits/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable runner"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/runner.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/runner.dir/build: runner

.PHONY : CMakeFiles/runner.dir/build

CMakeFiles/runner.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/runner.dir/cmake_clean.cmake
.PHONY : CMakeFiles/runner.dir/clean

CMakeFiles/runner.dir/depend:
	cd /home/morell/projects/HSE-Course-Private/module-1/lectures/lecture19/type_traits/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/morell/projects/HSE-Course-Private/module-1/lectures/lecture19/type_traits /home/morell/projects/HSE-Course-Private/module-1/lectures/lecture19/type_traits /home/morell/projects/HSE-Course-Private/module-1/lectures/lecture19/type_traits/build /home/morell/projects/HSE-Course-Private/module-1/lectures/lecture19/type_traits/build /home/morell/projects/HSE-Course-Private/module-1/lectures/lecture19/type_traits/build/CMakeFiles/runner.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/runner.dir/depend

