# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.25

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
CMAKE_COMMAND = /home/byeongseok/.local/lib/python3.6/site-packages/cmake/data/bin/cmake

# The command to remove a file.
RM = /home/byeongseok/.local/lib/python3.6/site-packages/cmake/data/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /mnt/c/users/inno-c-535/resource_moniter/memory_moniter

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /mnt/c/users/inno-c-535/resource_moniter/memory_moniter/cmake/build

# Include any dependencies generated for this target.
include CMakeFiles/hw_grpc_proto.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/hw_grpc_proto.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/hw_grpc_proto.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/hw_grpc_proto.dir/flags.make

moniter.pb.cc: /mnt/c/users/inno-c-535/resource_moniter/protos/moniter.proto
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/mnt/c/users/inno-c-535/resource_moniter/memory_moniter/cmake/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Generating moniter.pb.cc, moniter.pb.h, moniter.grpc.pb.cc, moniter.grpc.pb.h"
	/home/byeongseok/.local/bin/protoc-3.21.6.0 --grpc_out /mnt/c/users/inno-c-535/resource_moniter/memory_moniter/cmake/build --cpp_out /mnt/c/users/inno-c-535/resource_moniter/memory_moniter/cmake/build -I /mnt/c/users/inno-c-535/resource_moniter/protos --plugin=protoc-gen-grpc="/home/byeongseok/.local/bin/grpc_cpp_plugin" /mnt/c/users/inno-c-535/resource_moniter/protos/moniter.proto

moniter.pb.h: moniter.pb.cc
	@$(CMAKE_COMMAND) -E touch_nocreate moniter.pb.h

moniter.grpc.pb.cc: moniter.pb.cc
	@$(CMAKE_COMMAND) -E touch_nocreate moniter.grpc.pb.cc

moniter.grpc.pb.h: moniter.pb.cc
	@$(CMAKE_COMMAND) -E touch_nocreate moniter.grpc.pb.h

CMakeFiles/hw_grpc_proto.dir/moniter.grpc.pb.cc.o: CMakeFiles/hw_grpc_proto.dir/flags.make
CMakeFiles/hw_grpc_proto.dir/moniter.grpc.pb.cc.o: moniter.grpc.pb.cc
CMakeFiles/hw_grpc_proto.dir/moniter.grpc.pb.cc.o: CMakeFiles/hw_grpc_proto.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/mnt/c/users/inno-c-535/resource_moniter/memory_moniter/cmake/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/hw_grpc_proto.dir/moniter.grpc.pb.cc.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/hw_grpc_proto.dir/moniter.grpc.pb.cc.o -MF CMakeFiles/hw_grpc_proto.dir/moniter.grpc.pb.cc.o.d -o CMakeFiles/hw_grpc_proto.dir/moniter.grpc.pb.cc.o -c /mnt/c/users/inno-c-535/resource_moniter/memory_moniter/cmake/build/moniter.grpc.pb.cc

CMakeFiles/hw_grpc_proto.dir/moniter.grpc.pb.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/hw_grpc_proto.dir/moniter.grpc.pb.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /mnt/c/users/inno-c-535/resource_moniter/memory_moniter/cmake/build/moniter.grpc.pb.cc > CMakeFiles/hw_grpc_proto.dir/moniter.grpc.pb.cc.i

CMakeFiles/hw_grpc_proto.dir/moniter.grpc.pb.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/hw_grpc_proto.dir/moniter.grpc.pb.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /mnt/c/users/inno-c-535/resource_moniter/memory_moniter/cmake/build/moniter.grpc.pb.cc -o CMakeFiles/hw_grpc_proto.dir/moniter.grpc.pb.cc.s

CMakeFiles/hw_grpc_proto.dir/moniter.pb.cc.o: CMakeFiles/hw_grpc_proto.dir/flags.make
CMakeFiles/hw_grpc_proto.dir/moniter.pb.cc.o: moniter.pb.cc
CMakeFiles/hw_grpc_proto.dir/moniter.pb.cc.o: CMakeFiles/hw_grpc_proto.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/mnt/c/users/inno-c-535/resource_moniter/memory_moniter/cmake/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/hw_grpc_proto.dir/moniter.pb.cc.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/hw_grpc_proto.dir/moniter.pb.cc.o -MF CMakeFiles/hw_grpc_proto.dir/moniter.pb.cc.o.d -o CMakeFiles/hw_grpc_proto.dir/moniter.pb.cc.o -c /mnt/c/users/inno-c-535/resource_moniter/memory_moniter/cmake/build/moniter.pb.cc

CMakeFiles/hw_grpc_proto.dir/moniter.pb.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/hw_grpc_proto.dir/moniter.pb.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /mnt/c/users/inno-c-535/resource_moniter/memory_moniter/cmake/build/moniter.pb.cc > CMakeFiles/hw_grpc_proto.dir/moniter.pb.cc.i

CMakeFiles/hw_grpc_proto.dir/moniter.pb.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/hw_grpc_proto.dir/moniter.pb.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /mnt/c/users/inno-c-535/resource_moniter/memory_moniter/cmake/build/moniter.pb.cc -o CMakeFiles/hw_grpc_proto.dir/moniter.pb.cc.s

# Object files for target hw_grpc_proto
hw_grpc_proto_OBJECTS = \
"CMakeFiles/hw_grpc_proto.dir/moniter.grpc.pb.cc.o" \
"CMakeFiles/hw_grpc_proto.dir/moniter.pb.cc.o"

# External object files for target hw_grpc_proto
hw_grpc_proto_EXTERNAL_OBJECTS =

libhw_grpc_proto.a: CMakeFiles/hw_grpc_proto.dir/moniter.grpc.pb.cc.o
libhw_grpc_proto.a: CMakeFiles/hw_grpc_proto.dir/moniter.pb.cc.o
libhw_grpc_proto.a: CMakeFiles/hw_grpc_proto.dir/build.make
libhw_grpc_proto.a: CMakeFiles/hw_grpc_proto.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/mnt/c/users/inno-c-535/resource_moniter/memory_moniter/cmake/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking CXX static library libhw_grpc_proto.a"
	$(CMAKE_COMMAND) -P CMakeFiles/hw_grpc_proto.dir/cmake_clean_target.cmake
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/hw_grpc_proto.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/hw_grpc_proto.dir/build: libhw_grpc_proto.a
.PHONY : CMakeFiles/hw_grpc_proto.dir/build

CMakeFiles/hw_grpc_proto.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/hw_grpc_proto.dir/cmake_clean.cmake
.PHONY : CMakeFiles/hw_grpc_proto.dir/clean

CMakeFiles/hw_grpc_proto.dir/depend: moniter.grpc.pb.cc
CMakeFiles/hw_grpc_proto.dir/depend: moniter.grpc.pb.h
CMakeFiles/hw_grpc_proto.dir/depend: moniter.pb.cc
CMakeFiles/hw_grpc_proto.dir/depend: moniter.pb.h
	cd /mnt/c/users/inno-c-535/resource_moniter/memory_moniter/cmake/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /mnt/c/users/inno-c-535/resource_moniter/memory_moniter /mnt/c/users/inno-c-535/resource_moniter/memory_moniter /mnt/c/users/inno-c-535/resource_moniter/memory_moniter/cmake/build /mnt/c/users/inno-c-535/resource_moniter/memory_moniter/cmake/build /mnt/c/users/inno-c-535/resource_moniter/memory_moniter/cmake/build/CMakeFiles/hw_grpc_proto.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/hw_grpc_proto.dir/depend

