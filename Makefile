# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.22

# Default target executed when no arguments are given to make.
default_target: all
.PHONY : default_target

# Allow only one "make -f Makefile2" at a time, but pass parallelism.
.NOTPARALLEL:

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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/noah/Projects/TheNomad

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/noah/Projects/TheNomad

#=============================================================================
# Targets provided globally by CMake.

# Special rule for the target edit_cache
edit_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "No interactive CMake dialog available..."
	/usr/bin/cmake -E echo No\ interactive\ CMake\ dialog\ available.
.PHONY : edit_cache

# Special rule for the target edit_cache
edit_cache/fast: edit_cache
.PHONY : edit_cache/fast

# Special rule for the target rebuild_cache
rebuild_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Running CMake to regenerate build system..."
	/usr/bin/cmake --regenerate-during-build -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR)
.PHONY : rebuild_cache

# Special rule for the target rebuild_cache
rebuild_cache/fast: rebuild_cache
.PHONY : rebuild_cache/fast

# Special rule for the target list_install_components
list_install_components:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Available install components are: \"Unspecified\""
.PHONY : list_install_components

# Special rule for the target list_install_components
list_install_components/fast: list_install_components
.PHONY : list_install_components/fast

# Special rule for the target install
install: preinstall
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Install the project..."
	/usr/bin/cmake -P cmake_install.cmake
.PHONY : install

# Special rule for the target install
install/fast: preinstall/fast
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Install the project..."
	/usr/bin/cmake -P cmake_install.cmake
.PHONY : install/fast

# Special rule for the target install/local
install/local: preinstall
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Installing only the local directory..."
	/usr/bin/cmake -DCMAKE_INSTALL_LOCAL_ONLY=1 -P cmake_install.cmake
.PHONY : install/local

# Special rule for the target install/local
install/local/fast: preinstall/fast
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Installing only the local directory..."
	/usr/bin/cmake -DCMAKE_INSTALL_LOCAL_ONLY=1 -P cmake_install.cmake
.PHONY : install/local/fast

# Special rule for the target install/strip
install/strip: preinstall
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Installing the project stripped..."
	/usr/bin/cmake -DCMAKE_INSTALL_DO_STRIP=1 -P cmake_install.cmake
.PHONY : install/strip

# Special rule for the target install/strip
install/strip/fast: preinstall/fast
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Installing the project stripped..."
	/usr/bin/cmake -DCMAKE_INSTALL_DO_STRIP=1 -P cmake_install.cmake
.PHONY : install/strip/fast

# The main all target
all: cmake_check_build_system
	$(CMAKE_COMMAND) -E cmake_progress_start /home/noah/Projects/TheNomad/CMakeFiles /home/noah/Projects/TheNomad//CMakeFiles/progress.marks
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 all
	$(CMAKE_COMMAND) -E cmake_progress_start /home/noah/Projects/TheNomad/CMakeFiles 0
.PHONY : all

# The main clean target
clean:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 clean
.PHONY : clean

# The main clean target
clean/fast: clean
.PHONY : clean/fast

# Prepare targets for installation.
preinstall: all
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall

# Prepare targets for installation.
preinstall/fast:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall/fast

# clear depends
depend:
	$(CMAKE_COMMAND) -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 1
.PHONY : depend

#=============================================================================
# Target rules for targets named TheNomad.x64

# Build rule for target.
TheNomad.x64: cmake_check_build_system
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 TheNomad.x64
.PHONY : TheNomad.x64

# fast build rule for target.
TheNomad.x64/fast:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/TheNomad.x64.dir/build.make CMakeFiles/TheNomad.x64.dir/build
.PHONY : TheNomad.x64/fast

#=============================================================================
# Target rules for targets named shaders

# Build rule for target.
shaders: cmake_check_build_system
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 shaders
.PHONY : shaders

# fast build rule for target.
shaders/fast:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/shaders.dir/build.make CMakeFiles/shaders.dir/build
.PHONY : shaders/fast

#=============================================================================
# Target rules for targets named clean-all

# Build rule for target.
clean-all: cmake_check_build_system
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 clean-all
.PHONY : clean-all

# fast build rule for target.
clean-all/fast:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/clean-all.dir/build.make CMakeFiles/clean-all.dir/build
.PHONY : clean-all/fast

Source/Engine/Core/ConsoleManager.o: Source/Engine/Core/ConsoleManager.cpp.o
.PHONY : Source/Engine/Core/ConsoleManager.o

# target to build an object file
Source/Engine/Core/ConsoleManager.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/TheNomad.x64.dir/build.make CMakeFiles/TheNomad.x64.dir/Source/Engine/Core/ConsoleManager.cpp.o
.PHONY : Source/Engine/Core/ConsoleManager.cpp.o

Source/Engine/Core/ConsoleManager.i: Source/Engine/Core/ConsoleManager.cpp.i
.PHONY : Source/Engine/Core/ConsoleManager.i

# target to preprocess a source file
Source/Engine/Core/ConsoleManager.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/TheNomad.x64.dir/build.make CMakeFiles/TheNomad.x64.dir/Source/Engine/Core/ConsoleManager.cpp.i
.PHONY : Source/Engine/Core/ConsoleManager.cpp.i

Source/Engine/Core/ConsoleManager.s: Source/Engine/Core/ConsoleManager.cpp.s
.PHONY : Source/Engine/Core/ConsoleManager.s

# target to generate assembly for a file
Source/Engine/Core/ConsoleManager.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/TheNomad.x64.dir/build.make CMakeFiles/TheNomad.x64.dir/Source/Engine/Core/ConsoleManager.cpp.s
.PHONY : Source/Engine/Core/ConsoleManager.cpp.s

Source/Engine/Core/FileSystem/CachedFile.o: Source/Engine/Core/FileSystem/CachedFile.cpp.o
.PHONY : Source/Engine/Core/FileSystem/CachedFile.o

# target to build an object file
Source/Engine/Core/FileSystem/CachedFile.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/TheNomad.x64.dir/build.make CMakeFiles/TheNomad.x64.dir/Source/Engine/Core/FileSystem/CachedFile.cpp.o
.PHONY : Source/Engine/Core/FileSystem/CachedFile.cpp.o

Source/Engine/Core/FileSystem/CachedFile.i: Source/Engine/Core/FileSystem/CachedFile.cpp.i
.PHONY : Source/Engine/Core/FileSystem/CachedFile.i

# target to preprocess a source file
Source/Engine/Core/FileSystem/CachedFile.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/TheNomad.x64.dir/build.make CMakeFiles/TheNomad.x64.dir/Source/Engine/Core/FileSystem/CachedFile.cpp.i
.PHONY : Source/Engine/Core/FileSystem/CachedFile.cpp.i

Source/Engine/Core/FileSystem/CachedFile.s: Source/Engine/Core/FileSystem/CachedFile.cpp.s
.PHONY : Source/Engine/Core/FileSystem/CachedFile.s

# target to generate assembly for a file
Source/Engine/Core/FileSystem/CachedFile.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/TheNomad.x64.dir/build.make CMakeFiles/TheNomad.x64.dir/Source/Engine/Core/FileSystem/CachedFile.cpp.s
.PHONY : Source/Engine/Core/FileSystem/CachedFile.cpp.s

Source/Engine/Core/FileSystem/FileList.o: Source/Engine/Core/FileSystem/FileList.cpp.o
.PHONY : Source/Engine/Core/FileSystem/FileList.o

# target to build an object file
Source/Engine/Core/FileSystem/FileList.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/TheNomad.x64.dir/build.make CMakeFiles/TheNomad.x64.dir/Source/Engine/Core/FileSystem/FileList.cpp.o
.PHONY : Source/Engine/Core/FileSystem/FileList.cpp.o

Source/Engine/Core/FileSystem/FileList.i: Source/Engine/Core/FileSystem/FileList.cpp.i
.PHONY : Source/Engine/Core/FileSystem/FileList.i

# target to preprocess a source file
Source/Engine/Core/FileSystem/FileList.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/TheNomad.x64.dir/build.make CMakeFiles/TheNomad.x64.dir/Source/Engine/Core/FileSystem/FileList.cpp.i
.PHONY : Source/Engine/Core/FileSystem/FileList.cpp.i

Source/Engine/Core/FileSystem/FileList.s: Source/Engine/Core/FileSystem/FileList.cpp.s
.PHONY : Source/Engine/Core/FileSystem/FileList.s

# target to generate assembly for a file
Source/Engine/Core/FileSystem/FileList.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/TheNomad.x64.dir/build.make CMakeFiles/TheNomad.x64.dir/Source/Engine/Core/FileSystem/FileList.cpp.s
.PHONY : Source/Engine/Core/FileSystem/FileList.cpp.s

Source/Engine/Core/FileSystem/FileReader.o: Source/Engine/Core/FileSystem/FileReader.cpp.o
.PHONY : Source/Engine/Core/FileSystem/FileReader.o

# target to build an object file
Source/Engine/Core/FileSystem/FileReader.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/TheNomad.x64.dir/build.make CMakeFiles/TheNomad.x64.dir/Source/Engine/Core/FileSystem/FileReader.cpp.o
.PHONY : Source/Engine/Core/FileSystem/FileReader.cpp.o

Source/Engine/Core/FileSystem/FileReader.i: Source/Engine/Core/FileSystem/FileReader.cpp.i
.PHONY : Source/Engine/Core/FileSystem/FileReader.i

# target to preprocess a source file
Source/Engine/Core/FileSystem/FileReader.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/TheNomad.x64.dir/build.make CMakeFiles/TheNomad.x64.dir/Source/Engine/Core/FileSystem/FileReader.cpp.i
.PHONY : Source/Engine/Core/FileSystem/FileReader.cpp.i

Source/Engine/Core/FileSystem/FileReader.s: Source/Engine/Core/FileSystem/FileReader.cpp.s
.PHONY : Source/Engine/Core/FileSystem/FileReader.s

# target to generate assembly for a file
Source/Engine/Core/FileSystem/FileReader.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/TheNomad.x64.dir/build.make CMakeFiles/TheNomad.x64.dir/Source/Engine/Core/FileSystem/FileReader.cpp.s
.PHONY : Source/Engine/Core/FileSystem/FileReader.cpp.s

Source/Engine/Core/FileSystem/FileSystem.o: Source/Engine/Core/FileSystem/FileSystem.cpp.o
.PHONY : Source/Engine/Core/FileSystem/FileSystem.o

# target to build an object file
Source/Engine/Core/FileSystem/FileSystem.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/TheNomad.x64.dir/build.make CMakeFiles/TheNomad.x64.dir/Source/Engine/Core/FileSystem/FileSystem.cpp.o
.PHONY : Source/Engine/Core/FileSystem/FileSystem.cpp.o

Source/Engine/Core/FileSystem/FileSystem.i: Source/Engine/Core/FileSystem/FileSystem.cpp.i
.PHONY : Source/Engine/Core/FileSystem/FileSystem.i

# target to preprocess a source file
Source/Engine/Core/FileSystem/FileSystem.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/TheNomad.x64.dir/build.make CMakeFiles/TheNomad.x64.dir/Source/Engine/Core/FileSystem/FileSystem.cpp.i
.PHONY : Source/Engine/Core/FileSystem/FileSystem.cpp.i

Source/Engine/Core/FileSystem/FileSystem.s: Source/Engine/Core/FileSystem/FileSystem.cpp.s
.PHONY : Source/Engine/Core/FileSystem/FileSystem.s

# target to generate assembly for a file
Source/Engine/Core/FileSystem/FileSystem.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/TheNomad.x64.dir/build.make CMakeFiles/TheNomad.x64.dir/Source/Engine/Core/FileSystem/FileSystem.cpp.s
.PHONY : Source/Engine/Core/FileSystem/FileSystem.cpp.s

Source/Engine/Core/FileSystem/FileWriter.o: Source/Engine/Core/FileSystem/FileWriter.cpp.o
.PHONY : Source/Engine/Core/FileSystem/FileWriter.o

# target to build an object file
Source/Engine/Core/FileSystem/FileWriter.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/TheNomad.x64.dir/build.make CMakeFiles/TheNomad.x64.dir/Source/Engine/Core/FileSystem/FileWriter.cpp.o
.PHONY : Source/Engine/Core/FileSystem/FileWriter.cpp.o

Source/Engine/Core/FileSystem/FileWriter.i: Source/Engine/Core/FileSystem/FileWriter.cpp.i
.PHONY : Source/Engine/Core/FileSystem/FileWriter.i

# target to preprocess a source file
Source/Engine/Core/FileSystem/FileWriter.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/TheNomad.x64.dir/build.make CMakeFiles/TheNomad.x64.dir/Source/Engine/Core/FileSystem/FileWriter.cpp.i
.PHONY : Source/Engine/Core/FileSystem/FileWriter.cpp.i

Source/Engine/Core/FileSystem/FileWriter.s: Source/Engine/Core/FileSystem/FileWriter.cpp.s
.PHONY : Source/Engine/Core/FileSystem/FileWriter.s

# target to generate assembly for a file
Source/Engine/Core/FileSystem/FileWriter.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/TheNomad.x64.dir/build.make CMakeFiles/TheNomad.x64.dir/Source/Engine/Core/FileSystem/FileWriter.cpp.s
.PHONY : Source/Engine/Core/FileSystem/FileWriter.cpp.s

Source/Engine/Core/Logging/Logger.o: Source/Engine/Core/Logging/Logger.cpp.o
.PHONY : Source/Engine/Core/Logging/Logger.o

# target to build an object file
Source/Engine/Core/Logging/Logger.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/TheNomad.x64.dir/build.make CMakeFiles/TheNomad.x64.dir/Source/Engine/Core/Logging/Logger.cpp.o
.PHONY : Source/Engine/Core/Logging/Logger.cpp.o

Source/Engine/Core/Logging/Logger.i: Source/Engine/Core/Logging/Logger.cpp.i
.PHONY : Source/Engine/Core/Logging/Logger.i

# target to preprocess a source file
Source/Engine/Core/Logging/Logger.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/TheNomad.x64.dir/build.make CMakeFiles/TheNomad.x64.dir/Source/Engine/Core/Logging/Logger.cpp.i
.PHONY : Source/Engine/Core/Logging/Logger.cpp.i

Source/Engine/Core/Logging/Logger.s: Source/Engine/Core/Logging/Logger.cpp.s
.PHONY : Source/Engine/Core/Logging/Logger.s

# target to generate assembly for a file
Source/Engine/Core/Logging/Logger.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/TheNomad.x64.dir/build.make CMakeFiles/TheNomad.x64.dir/Source/Engine/Core/Logging/Logger.cpp.s
.PHONY : Source/Engine/Core/Logging/Logger.cpp.s

Source/Engine/Core/Serialization/Ini/IniReader.o: Source/Engine/Core/Serialization/Ini/IniReader.cpp.o
.PHONY : Source/Engine/Core/Serialization/Ini/IniReader.o

# target to build an object file
Source/Engine/Core/Serialization/Ini/IniReader.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/TheNomad.x64.dir/build.make CMakeFiles/TheNomad.x64.dir/Source/Engine/Core/Serialization/Ini/IniReader.cpp.o
.PHONY : Source/Engine/Core/Serialization/Ini/IniReader.cpp.o

Source/Engine/Core/Serialization/Ini/IniReader.i: Source/Engine/Core/Serialization/Ini/IniReader.cpp.i
.PHONY : Source/Engine/Core/Serialization/Ini/IniReader.i

# target to preprocess a source file
Source/Engine/Core/Serialization/Ini/IniReader.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/TheNomad.x64.dir/build.make CMakeFiles/TheNomad.x64.dir/Source/Engine/Core/Serialization/Ini/IniReader.cpp.i
.PHONY : Source/Engine/Core/Serialization/Ini/IniReader.cpp.i

Source/Engine/Core/Serialization/Ini/IniReader.s: Source/Engine/Core/Serialization/Ini/IniReader.cpp.s
.PHONY : Source/Engine/Core/Serialization/Ini/IniReader.s

# target to generate assembly for a file
Source/Engine/Core/Serialization/Ini/IniReader.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/TheNomad.x64.dir/build.make CMakeFiles/TheNomad.x64.dir/Source/Engine/Core/Serialization/Ini/IniReader.cpp.s
.PHONY : Source/Engine/Core/Serialization/Ini/IniReader.cpp.s

Source/Engine/Core/Serialization/Ini/IniWriter.o: Source/Engine/Core/Serialization/Ini/IniWriter.cpp.o
.PHONY : Source/Engine/Core/Serialization/Ini/IniWriter.o

# target to build an object file
Source/Engine/Core/Serialization/Ini/IniWriter.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/TheNomad.x64.dir/build.make CMakeFiles/TheNomad.x64.dir/Source/Engine/Core/Serialization/Ini/IniWriter.cpp.o
.PHONY : Source/Engine/Core/Serialization/Ini/IniWriter.cpp.o

Source/Engine/Core/Serialization/Ini/IniWriter.i: Source/Engine/Core/Serialization/Ini/IniWriter.cpp.i
.PHONY : Source/Engine/Core/Serialization/Ini/IniWriter.i

# target to preprocess a source file
Source/Engine/Core/Serialization/Ini/IniWriter.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/TheNomad.x64.dir/build.make CMakeFiles/TheNomad.x64.dir/Source/Engine/Core/Serialization/Ini/IniWriter.cpp.i
.PHONY : Source/Engine/Core/Serialization/Ini/IniWriter.cpp.i

Source/Engine/Core/Serialization/Ini/IniWriter.s: Source/Engine/Core/Serialization/Ini/IniWriter.cpp.s
.PHONY : Source/Engine/Core/Serialization/Ini/IniWriter.s

# target to generate assembly for a file
Source/Engine/Core/Serialization/Ini/IniWriter.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/TheNomad.x64.dir/build.make CMakeFiles/TheNomad.x64.dir/Source/Engine/Core/Serialization/Ini/IniWriter.cpp.s
.PHONY : Source/Engine/Core/Serialization/Ini/IniWriter.cpp.s

Source/Engine/Core/Serialization/Ini/ini.o: Source/Engine/Core/Serialization/Ini/ini.cpp.o
.PHONY : Source/Engine/Core/Serialization/Ini/ini.o

# target to build an object file
Source/Engine/Core/Serialization/Ini/ini.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/TheNomad.x64.dir/build.make CMakeFiles/TheNomad.x64.dir/Source/Engine/Core/Serialization/Ini/ini.cpp.o
.PHONY : Source/Engine/Core/Serialization/Ini/ini.cpp.o

Source/Engine/Core/Serialization/Ini/ini.i: Source/Engine/Core/Serialization/Ini/ini.cpp.i
.PHONY : Source/Engine/Core/Serialization/Ini/ini.i

# target to preprocess a source file
Source/Engine/Core/Serialization/Ini/ini.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/TheNomad.x64.dir/build.make CMakeFiles/TheNomad.x64.dir/Source/Engine/Core/Serialization/Ini/ini.cpp.i
.PHONY : Source/Engine/Core/Serialization/Ini/ini.cpp.i

Source/Engine/Core/Serialization/Ini/ini.s: Source/Engine/Core/Serialization/Ini/ini.cpp.s
.PHONY : Source/Engine/Core/Serialization/Ini/ini.s

# target to generate assembly for a file
Source/Engine/Core/Serialization/Ini/ini.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/TheNomad.x64.dir/build.make CMakeFiles/TheNomad.x64.dir/Source/Engine/Core/Serialization/Ini/ini.cpp.s
.PHONY : Source/Engine/Core/Serialization/Ini/ini.cpp.s

Source/Engine/Core/System/GenericApplication/GenericApplication.o: Source/Engine/Core/System/GenericApplication/GenericApplication.cpp.o
.PHONY : Source/Engine/Core/System/GenericApplication/GenericApplication.o

# target to build an object file
Source/Engine/Core/System/GenericApplication/GenericApplication.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/TheNomad.x64.dir/build.make CMakeFiles/TheNomad.x64.dir/Source/Engine/Core/System/GenericApplication/GenericApplication.cpp.o
.PHONY : Source/Engine/Core/System/GenericApplication/GenericApplication.cpp.o

Source/Engine/Core/System/GenericApplication/GenericApplication.i: Source/Engine/Core/System/GenericApplication/GenericApplication.cpp.i
.PHONY : Source/Engine/Core/System/GenericApplication/GenericApplication.i

# target to preprocess a source file
Source/Engine/Core/System/GenericApplication/GenericApplication.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/TheNomad.x64.dir/build.make CMakeFiles/TheNomad.x64.dir/Source/Engine/Core/System/GenericApplication/GenericApplication.cpp.i
.PHONY : Source/Engine/Core/System/GenericApplication/GenericApplication.cpp.i

Source/Engine/Core/System/GenericApplication/GenericApplication.s: Source/Engine/Core/System/GenericApplication/GenericApplication.cpp.s
.PHONY : Source/Engine/Core/System/GenericApplication/GenericApplication.s

# target to generate assembly for a file
Source/Engine/Core/System/GenericApplication/GenericApplication.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/TheNomad.x64.dir/build.make CMakeFiles/TheNomad.x64.dir/Source/Engine/Core/System/GenericApplication/GenericApplication.cpp.s
.PHONY : Source/Engine/Core/System/GenericApplication/GenericApplication.cpp.s

Source/Engine/Core/System/Posix/PosixApplication.o: Source/Engine/Core/System/Posix/PosixApplication.cpp.o
.PHONY : Source/Engine/Core/System/Posix/PosixApplication.o

# target to build an object file
Source/Engine/Core/System/Posix/PosixApplication.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/TheNomad.x64.dir/build.make CMakeFiles/TheNomad.x64.dir/Source/Engine/Core/System/Posix/PosixApplication.cpp.o
.PHONY : Source/Engine/Core/System/Posix/PosixApplication.cpp.o

Source/Engine/Core/System/Posix/PosixApplication.i: Source/Engine/Core/System/Posix/PosixApplication.cpp.i
.PHONY : Source/Engine/Core/System/Posix/PosixApplication.i

# target to preprocess a source file
Source/Engine/Core/System/Posix/PosixApplication.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/TheNomad.x64.dir/build.make CMakeFiles/TheNomad.x64.dir/Source/Engine/Core/System/Posix/PosixApplication.cpp.i
.PHONY : Source/Engine/Core/System/Posix/PosixApplication.cpp.i

Source/Engine/Core/System/Posix/PosixApplication.s: Source/Engine/Core/System/Posix/PosixApplication.cpp.s
.PHONY : Source/Engine/Core/System/Posix/PosixApplication.s

# target to generate assembly for a file
Source/Engine/Core/System/Posix/PosixApplication.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/TheNomad.x64.dir/build.make CMakeFiles/TheNomad.x64.dir/Source/Engine/Core/System/Posix/PosixApplication.cpp.s
.PHONY : Source/Engine/Core/System/Posix/PosixApplication.cpp.s

Source/Engine/Core/ThreadSystem/Threads.o: Source/Engine/Core/ThreadSystem/Threads.cpp.o
.PHONY : Source/Engine/Core/ThreadSystem/Threads.o

# target to build an object file
Source/Engine/Core/ThreadSystem/Threads.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/TheNomad.x64.dir/build.make CMakeFiles/TheNomad.x64.dir/Source/Engine/Core/ThreadSystem/Threads.cpp.o
.PHONY : Source/Engine/Core/ThreadSystem/Threads.cpp.o

Source/Engine/Core/ThreadSystem/Threads.i: Source/Engine/Core/ThreadSystem/Threads.cpp.i
.PHONY : Source/Engine/Core/ThreadSystem/Threads.i

# target to preprocess a source file
Source/Engine/Core/ThreadSystem/Threads.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/TheNomad.x64.dir/build.make CMakeFiles/TheNomad.x64.dir/Source/Engine/Core/ThreadSystem/Threads.cpp.i
.PHONY : Source/Engine/Core/ThreadSystem/Threads.cpp.i

Source/Engine/Core/ThreadSystem/Threads.s: Source/Engine/Core/ThreadSystem/Threads.cpp.s
.PHONY : Source/Engine/Core/ThreadSystem/Threads.s

# target to generate assembly for a file
Source/Engine/Core/ThreadSystem/Threads.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/TheNomad.x64.dir/build.make CMakeFiles/TheNomad.x64.dir/Source/Engine/Core/ThreadSystem/Threads.cpp.s
.PHONY : Source/Engine/Core/ThreadSystem/Threads.cpp.s

Source/Engine/Core/Util.o: Source/Engine/Core/Util.cpp.o
.PHONY : Source/Engine/Core/Util.o

# target to build an object file
Source/Engine/Core/Util.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/TheNomad.x64.dir/build.make CMakeFiles/TheNomad.x64.dir/Source/Engine/Core/Util.cpp.o
.PHONY : Source/Engine/Core/Util.cpp.o

Source/Engine/Core/Util.i: Source/Engine/Core/Util.cpp.i
.PHONY : Source/Engine/Core/Util.i

# target to preprocess a source file
Source/Engine/Core/Util.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/TheNomad.x64.dir/build.make CMakeFiles/TheNomad.x64.dir/Source/Engine/Core/Util.cpp.i
.PHONY : Source/Engine/Core/Util.cpp.i

Source/Engine/Core/Util.s: Source/Engine/Core/Util.cpp.s
.PHONY : Source/Engine/Core/Util.s

# target to generate assembly for a file
Source/Engine/Core/Util.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/TheNomad.x64.dir/build.make CMakeFiles/TheNomad.x64.dir/Source/Engine/Core/Util.cpp.s
.PHONY : Source/Engine/Core/Util.cpp.s

Source/Engine/RenderLib/Backend/RenderContext.o: Source/Engine/RenderLib/Backend/RenderContext.cpp.o
.PHONY : Source/Engine/RenderLib/Backend/RenderContext.o

# target to build an object file
Source/Engine/RenderLib/Backend/RenderContext.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/TheNomad.x64.dir/build.make CMakeFiles/TheNomad.x64.dir/Source/Engine/RenderLib/Backend/RenderContext.cpp.o
.PHONY : Source/Engine/RenderLib/Backend/RenderContext.cpp.o

Source/Engine/RenderLib/Backend/RenderContext.i: Source/Engine/RenderLib/Backend/RenderContext.cpp.i
.PHONY : Source/Engine/RenderLib/Backend/RenderContext.i

# target to preprocess a source file
Source/Engine/RenderLib/Backend/RenderContext.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/TheNomad.x64.dir/build.make CMakeFiles/TheNomad.x64.dir/Source/Engine/RenderLib/Backend/RenderContext.cpp.i
.PHONY : Source/Engine/RenderLib/Backend/RenderContext.cpp.i

Source/Engine/RenderLib/Backend/RenderContext.s: Source/Engine/RenderLib/Backend/RenderContext.cpp.s
.PHONY : Source/Engine/RenderLib/Backend/RenderContext.s

# target to generate assembly for a file
Source/Engine/RenderLib/Backend/RenderContext.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/TheNomad.x64.dir/build.make CMakeFiles/TheNomad.x64.dir/Source/Engine/RenderLib/Backend/RenderContext.cpp.s
.PHONY : Source/Engine/RenderLib/Backend/RenderContext.cpp.s

Source/Engine/RenderLib/Backend/Vulkan/VulkanContext.o: Source/Engine/RenderLib/Backend/Vulkan/VulkanContext.cpp.o
.PHONY : Source/Engine/RenderLib/Backend/Vulkan/VulkanContext.o

# target to build an object file
Source/Engine/RenderLib/Backend/Vulkan/VulkanContext.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/TheNomad.x64.dir/build.make CMakeFiles/TheNomad.x64.dir/Source/Engine/RenderLib/Backend/Vulkan/VulkanContext.cpp.o
.PHONY : Source/Engine/RenderLib/Backend/Vulkan/VulkanContext.cpp.o

Source/Engine/RenderLib/Backend/Vulkan/VulkanContext.i: Source/Engine/RenderLib/Backend/Vulkan/VulkanContext.cpp.i
.PHONY : Source/Engine/RenderLib/Backend/Vulkan/VulkanContext.i

# target to preprocess a source file
Source/Engine/RenderLib/Backend/Vulkan/VulkanContext.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/TheNomad.x64.dir/build.make CMakeFiles/TheNomad.x64.dir/Source/Engine/RenderLib/Backend/Vulkan/VulkanContext.cpp.i
.PHONY : Source/Engine/RenderLib/Backend/Vulkan/VulkanContext.cpp.i

Source/Engine/RenderLib/Backend/Vulkan/VulkanContext.s: Source/Engine/RenderLib/Backend/Vulkan/VulkanContext.cpp.s
.PHONY : Source/Engine/RenderLib/Backend/Vulkan/VulkanContext.s

# target to generate assembly for a file
Source/Engine/RenderLib/Backend/Vulkan/VulkanContext.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/TheNomad.x64.dir/build.make CMakeFiles/TheNomad.x64.dir/Source/Engine/RenderLib/Backend/Vulkan/VulkanContext.cpp.s
.PHONY : Source/Engine/RenderLib/Backend/Vulkan/VulkanContext.cpp.s

Source/Engine/RenderLib/Backend/Vulkan/VulkanShader.o: Source/Engine/RenderLib/Backend/Vulkan/VulkanShader.cpp.o
.PHONY : Source/Engine/RenderLib/Backend/Vulkan/VulkanShader.o

# target to build an object file
Source/Engine/RenderLib/Backend/Vulkan/VulkanShader.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/TheNomad.x64.dir/build.make CMakeFiles/TheNomad.x64.dir/Source/Engine/RenderLib/Backend/Vulkan/VulkanShader.cpp.o
.PHONY : Source/Engine/RenderLib/Backend/Vulkan/VulkanShader.cpp.o

Source/Engine/RenderLib/Backend/Vulkan/VulkanShader.i: Source/Engine/RenderLib/Backend/Vulkan/VulkanShader.cpp.i
.PHONY : Source/Engine/RenderLib/Backend/Vulkan/VulkanShader.i

# target to preprocess a source file
Source/Engine/RenderLib/Backend/Vulkan/VulkanShader.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/TheNomad.x64.dir/build.make CMakeFiles/TheNomad.x64.dir/Source/Engine/RenderLib/Backend/Vulkan/VulkanShader.cpp.i
.PHONY : Source/Engine/RenderLib/Backend/Vulkan/VulkanShader.cpp.i

Source/Engine/RenderLib/Backend/Vulkan/VulkanShader.s: Source/Engine/RenderLib/Backend/Vulkan/VulkanShader.cpp.s
.PHONY : Source/Engine/RenderLib/Backend/Vulkan/VulkanShader.s

# target to generate assembly for a file
Source/Engine/RenderLib/Backend/Vulkan/VulkanShader.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/TheNomad.x64.dir/build.make CMakeFiles/TheNomad.x64.dir/Source/Engine/RenderLib/Backend/Vulkan/VulkanShader.cpp.s
.PHONY : Source/Engine/RenderLib/Backend/Vulkan/VulkanShader.cpp.s

# Help Target
help:
	@echo "The following are some of the valid targets for this Makefile:"
	@echo "... all (the default if no target is provided)"
	@echo "... clean"
	@echo "... depend"
	@echo "... edit_cache"
	@echo "... install"
	@echo "... install/local"
	@echo "... install/strip"
	@echo "... list_install_components"
	@echo "... rebuild_cache"
	@echo "... clean-all"
	@echo "... shaders"
	@echo "... TheNomad.x64"
	@echo "... Source/Engine/Core/ConsoleManager.o"
	@echo "... Source/Engine/Core/ConsoleManager.i"
	@echo "... Source/Engine/Core/ConsoleManager.s"
	@echo "... Source/Engine/Core/FileSystem/CachedFile.o"
	@echo "... Source/Engine/Core/FileSystem/CachedFile.i"
	@echo "... Source/Engine/Core/FileSystem/CachedFile.s"
	@echo "... Source/Engine/Core/FileSystem/FileList.o"
	@echo "... Source/Engine/Core/FileSystem/FileList.i"
	@echo "... Source/Engine/Core/FileSystem/FileList.s"
	@echo "... Source/Engine/Core/FileSystem/FileReader.o"
	@echo "... Source/Engine/Core/FileSystem/FileReader.i"
	@echo "... Source/Engine/Core/FileSystem/FileReader.s"
	@echo "... Source/Engine/Core/FileSystem/FileSystem.o"
	@echo "... Source/Engine/Core/FileSystem/FileSystem.i"
	@echo "... Source/Engine/Core/FileSystem/FileSystem.s"
	@echo "... Source/Engine/Core/FileSystem/FileWriter.o"
	@echo "... Source/Engine/Core/FileSystem/FileWriter.i"
	@echo "... Source/Engine/Core/FileSystem/FileWriter.s"
	@echo "... Source/Engine/Core/Logging/Logger.o"
	@echo "... Source/Engine/Core/Logging/Logger.i"
	@echo "... Source/Engine/Core/Logging/Logger.s"
	@echo "... Source/Engine/Core/Serialization/Ini/IniReader.o"
	@echo "... Source/Engine/Core/Serialization/Ini/IniReader.i"
	@echo "... Source/Engine/Core/Serialization/Ini/IniReader.s"
	@echo "... Source/Engine/Core/Serialization/Ini/IniWriter.o"
	@echo "... Source/Engine/Core/Serialization/Ini/IniWriter.i"
	@echo "... Source/Engine/Core/Serialization/Ini/IniWriter.s"
	@echo "... Source/Engine/Core/Serialization/Ini/ini.o"
	@echo "... Source/Engine/Core/Serialization/Ini/ini.i"
	@echo "... Source/Engine/Core/Serialization/Ini/ini.s"
	@echo "... Source/Engine/Core/System/GenericApplication/GenericApplication.o"
	@echo "... Source/Engine/Core/System/GenericApplication/GenericApplication.i"
	@echo "... Source/Engine/Core/System/GenericApplication/GenericApplication.s"
	@echo "... Source/Engine/Core/System/Posix/PosixApplication.o"
	@echo "... Source/Engine/Core/System/Posix/PosixApplication.i"
	@echo "... Source/Engine/Core/System/Posix/PosixApplication.s"
	@echo "... Source/Engine/Core/ThreadSystem/Threads.o"
	@echo "... Source/Engine/Core/ThreadSystem/Threads.i"
	@echo "... Source/Engine/Core/ThreadSystem/Threads.s"
	@echo "... Source/Engine/Core/Util.o"
	@echo "... Source/Engine/Core/Util.i"
	@echo "... Source/Engine/Core/Util.s"
	@echo "... Source/Engine/RenderLib/Backend/RenderContext.o"
	@echo "... Source/Engine/RenderLib/Backend/RenderContext.i"
	@echo "... Source/Engine/RenderLib/Backend/RenderContext.s"
	@echo "... Source/Engine/RenderLib/Backend/Vulkan/VulkanContext.o"
	@echo "... Source/Engine/RenderLib/Backend/Vulkan/VulkanContext.i"
	@echo "... Source/Engine/RenderLib/Backend/Vulkan/VulkanContext.s"
	@echo "... Source/Engine/RenderLib/Backend/Vulkan/VulkanShader.o"
	@echo "... Source/Engine/RenderLib/Backend/Vulkan/VulkanShader.i"
	@echo "... Source/Engine/RenderLib/Backend/Vulkan/VulkanShader.s"
.PHONY : help



#=============================================================================
# Special targets to cleanup operation of make.

# Special rule to run CMake to check the build system integrity.
# No rule that depends on this can have commands that come from listfiles
# because they might be regenerated.
cmake_check_build_system:
	$(CMAKE_COMMAND) -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 0
.PHONY : cmake_check_build_system

