# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.14

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

SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = "D:\Programmi\JetBrains\CLion 2019.1.3\bin\cmake\win\bin\cmake.exe"

# The command to remove a file.
RM = "D:\Programmi\JetBrains\CLion 2019.1.3\bin\cmake\win\bin\cmake.exe" -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = D:\Raffaele\Documenti\CLion\PDS_Client

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = D:\Raffaele\Documenti\CLion\PDS_Client\cmake-build-debug

# Utility rule file for PDS_Client_autogen.

# Include the progress variables for this target.
include CMakeFiles/PDS_Client_autogen.dir/progress.make

CMakeFiles/PDS_Client_autogen:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=D:\Raffaele\Documenti\CLion\PDS_Client\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Automatic MOC and UIC for target PDS_Client"
	"D:\Programmi\JetBrains\CLion 2019.1.3\bin\cmake\win\bin\cmake.exe" -E cmake_autogen D:/Raffaele/Documenti/CLion/PDS_Client/cmake-build-debug/CMakeFiles/PDS_Client_autogen.dir/AutogenInfo.cmake Debug

PDS_Client_autogen: CMakeFiles/PDS_Client_autogen
PDS_Client_autogen: CMakeFiles/PDS_Client_autogen.dir/build.make

.PHONY : PDS_Client_autogen

# Rule to build all files generated by this target.
CMakeFiles/PDS_Client_autogen.dir/build: PDS_Client_autogen

.PHONY : CMakeFiles/PDS_Client_autogen.dir/build

CMakeFiles/PDS_Client_autogen.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles\PDS_Client_autogen.dir\cmake_clean.cmake
.PHONY : CMakeFiles/PDS_Client_autogen.dir/clean

CMakeFiles/PDS_Client_autogen.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" D:\Raffaele\Documenti\CLion\PDS_Client D:\Raffaele\Documenti\CLion\PDS_Client D:\Raffaele\Documenti\CLion\PDS_Client\cmake-build-debug D:\Raffaele\Documenti\CLion\PDS_Client\cmake-build-debug D:\Raffaele\Documenti\CLion\PDS_Client\cmake-build-debug\CMakeFiles\PDS_Client_autogen.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/PDS_Client_autogen.dir/depend

