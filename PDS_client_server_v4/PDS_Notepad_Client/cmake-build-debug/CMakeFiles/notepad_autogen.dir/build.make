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
CMAKE_SOURCE_DIR = D:\Raffaele\Documenti\GitHub\PDS_Project\PDS_client_server_v3\PDS_Notepad_Client

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = D:\Raffaele\Documenti\GitHub\PDS_Project\PDS_client_server_v3\PDS_Notepad_Client\cmake-build-debug

# Utility rule file for notepad_autogen.

# Include the progress variables for this target.
include CMakeFiles/notepad_autogen.dir/progress.make

CMakeFiles/notepad_autogen:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=D:\Raffaele\Documenti\GitHub\PDS_Project\PDS_client_server_v3\PDS_Notepad_Client\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Automatic MOC and UIC for target notepad"
	"D:\Programmi\JetBrains\CLion 2019.1.3\bin\cmake\win\bin\cmake.exe" -E cmake_autogen D:/Raffaele/Documenti/GitHub/PDS_Project/PDS_client_server_v3/PDS_Notepad_Client/cmake-build-debug/CMakeFiles/notepad_autogen.dir/AutogenInfo.cmake Debug

notepad_autogen: CMakeFiles/notepad_autogen
notepad_autogen: CMakeFiles/notepad_autogen.dir/build.make

.PHONY : notepad_autogen

# Rule to build all files generated by this target.
CMakeFiles/notepad_autogen.dir/build: notepad_autogen

.PHONY : CMakeFiles/notepad_autogen.dir/build

CMakeFiles/notepad_autogen.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles\notepad_autogen.dir\cmake_clean.cmake
.PHONY : CMakeFiles/notepad_autogen.dir/clean

CMakeFiles/notepad_autogen.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" D:\Raffaele\Documenti\GitHub\PDS_Project\PDS_client_server_v3\PDS_Notepad_Client D:\Raffaele\Documenti\GitHub\PDS_Project\PDS_client_server_v3\PDS_Notepad_Client D:\Raffaele\Documenti\GitHub\PDS_Project\PDS_client_server_v3\PDS_Notepad_Client\cmake-build-debug D:\Raffaele\Documenti\GitHub\PDS_Project\PDS_client_server_v3\PDS_Notepad_Client\cmake-build-debug D:\Raffaele\Documenti\GitHub\PDS_Project\PDS_client_server_v3\PDS_Notepad_Client\cmake-build-debug\CMakeFiles\notepad_autogen.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/notepad_autogen.dir/depend

