# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.30

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
CMAKE_COMMAND = /usr/local/cmake-3.30.2-linux-aarch64/bin/cmake

# The command to remove a file.
RM = /usr/local/cmake-3.30.2-linux-aarch64/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /data/chw/mempool_chw

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /data/chw/mempool_chw/build

# Include any dependencies generated for this target.
include app/CMakeFiles/app.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include app/CMakeFiles/app.dir/compiler_depend.make

# Include the progress variables for this target.
include app/CMakeFiles/app.dir/progress.make

# Include the compile flags for this target's objects.
include app/CMakeFiles/app.dir/flags.make

app/CMakeFiles/app.dir/app.cpp.o: app/CMakeFiles/app.dir/flags.make
app/CMakeFiles/app.dir/app.cpp.o: /data/chw/mempool_chw/app/app.cpp
app/CMakeFiles/app.dir/app.cpp.o: app/CMakeFiles/app.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/data/chw/mempool_chw/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object app/CMakeFiles/app.dir/app.cpp.o"
	cd /data/chw/mempool_chw/build/app && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT app/CMakeFiles/app.dir/app.cpp.o -MF CMakeFiles/app.dir/app.cpp.o.d -o CMakeFiles/app.dir/app.cpp.o -c /data/chw/mempool_chw/app/app.cpp

app/CMakeFiles/app.dir/app.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/app.dir/app.cpp.i"
	cd /data/chw/mempool_chw/build/app && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /data/chw/mempool_chw/app/app.cpp > CMakeFiles/app.dir/app.cpp.i

app/CMakeFiles/app.dir/app.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/app.dir/app.cpp.s"
	cd /data/chw/mempool_chw/build/app && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /data/chw/mempool_chw/app/app.cpp -o CMakeFiles/app.dir/app.cpp.s

app/CMakeFiles/app.dir/main.cpp.o: app/CMakeFiles/app.dir/flags.make
app/CMakeFiles/app.dir/main.cpp.o: /data/chw/mempool_chw/app/main.cpp
app/CMakeFiles/app.dir/main.cpp.o: app/CMakeFiles/app.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/data/chw/mempool_chw/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object app/CMakeFiles/app.dir/main.cpp.o"
	cd /data/chw/mempool_chw/build/app && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT app/CMakeFiles/app.dir/main.cpp.o -MF CMakeFiles/app.dir/main.cpp.o.d -o CMakeFiles/app.dir/main.cpp.o -c /data/chw/mempool_chw/app/main.cpp

app/CMakeFiles/app.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/app.dir/main.cpp.i"
	cd /data/chw/mempool_chw/build/app && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /data/chw/mempool_chw/app/main.cpp > CMakeFiles/app.dir/main.cpp.i

app/CMakeFiles/app.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/app.dir/main.cpp.s"
	cd /data/chw/mempool_chw/build/app && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /data/chw/mempool_chw/app/main.cpp -o CMakeFiles/app.dir/main.cpp.s

# Object files for target app
app_OBJECTS = \
"CMakeFiles/app.dir/app.cpp.o" \
"CMakeFiles/app.dir/main.cpp.o"

# External object files for target app
app_EXTERNAL_OBJECTS =

/data/chw/mempool_chw/app/bin/app: app/CMakeFiles/app.dir/app.cpp.o
/data/chw/mempool_chw/app/bin/app: app/CMakeFiles/app.dir/main.cpp.o
/data/chw/mempool_chw/app/bin/app: app/CMakeFiles/app.dir/build.make
/data/chw/mempool_chw/app/bin/app: /data/chw/mempool_chw/lib/libnvis.so.1.0.0
/data/chw/mempool_chw/app/bin/app: /data/chw/mempool_chw/3rdparty/opencv/lib/linux_lib/libopencv_core.so
/data/chw/mempool_chw/app/bin/app: /data/chw/mempool_chw/3rdparty/opencv/lib/linux_lib/libopencv_highgui.so
/data/chw/mempool_chw/app/bin/app: /data/chw/mempool_chw/3rdparty/opencv/lib/linux_lib/libopencv_imgproc.so
/data/chw/mempool_chw/app/bin/app: /data/chw/mempool_chw/3rdparty/opencv/lib/linux_lib/libopencv_imgcodecs.so
/data/chw/mempool_chw/app/bin/app: /data/chw/mempool_chw/3rdparty/opencv/lib/linux_lib/libopencv_video.so
/data/chw/mempool_chw/app/bin/app: /data/chw/mempool_chw/3rdparty/opencv/lib/linux_lib/libopencv_videoio.so
/data/chw/mempool_chw/app/bin/app: /data/chw/mempool_chw/3rdparty/opencv/lib/linux_lib/libopencv_features2d.so
/data/chw/mempool_chw/app/bin/app: /data/chw/mempool_chw/3rdparty/opencv/lib/linux_lib/libopencv_flann.so
/data/chw/mempool_chw/app/bin/app: /data/chw/mempool_chw/3rdparty/opencv/lib/linux_lib/libopencv_calib3d.so
/data/chw/mempool_chw/app/bin/app: /data/chw/mempool_chw/3rdparty/opencv/lib/linux_lib/libopencv_dnn.so
/data/chw/mempool_chw/app/bin/app: /data/chw/mempool_chw/3rdparty/gflags/lib/linux_lib/libgflags.so
/data/chw/mempool_chw/app/bin/app: /data/chw/mempool_chw/3rdparty/glog/lib/linux_lib/libglog.so
/data/chw/mempool_chw/app/bin/app: app/CMakeFiles/app.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/data/chw/mempool_chw/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX executable /data/chw/mempool_chw/app/bin/app"
	cd /data/chw/mempool_chw/build/app && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/app.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
app/CMakeFiles/app.dir/build: /data/chw/mempool_chw/app/bin/app
.PHONY : app/CMakeFiles/app.dir/build

app/CMakeFiles/app.dir/clean:
	cd /data/chw/mempool_chw/build/app && $(CMAKE_COMMAND) -P CMakeFiles/app.dir/cmake_clean.cmake
.PHONY : app/CMakeFiles/app.dir/clean

app/CMakeFiles/app.dir/depend:
	cd /data/chw/mempool_chw/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /data/chw/mempool_chw /data/chw/mempool_chw/app /data/chw/mempool_chw/build /data/chw/mempool_chw/build/app /data/chw/mempool_chw/build/app/CMakeFiles/app.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : app/CMakeFiles/app.dir/depend

