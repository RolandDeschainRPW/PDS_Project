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
CMAKE_SOURCE_DIR = D:\Raffaele\Documenti\GitHub\PDS_Project\Notepad_Project\CLion\notepad

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = D:\Raffaele\Documenti\GitHub\PDS_Project\Notepad_Project\CLion\notepad\cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/notepad.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/notepad.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/notepad.dir/flags.make

notepad_autogen/EWIEGA46WW/qrc_notepad.cpp: ../notepad.qrc
notepad_autogen/EWIEGA46WW/qrc_notepad.cpp: CMakeFiles/notepad_autogen.dir/RCCnotepadInfo.cmake
notepad_autogen/EWIEGA46WW/qrc_notepad.cpp: ../images/open.png
notepad_autogen/EWIEGA46WW/qrc_notepad.cpp: ../images/underline.png
notepad_autogen/EWIEGA46WW/qrc_notepad.cpp: ../images/font.png
notepad_autogen/EWIEGA46WW/qrc_notepad.cpp: ../images/paste.png
notepad_autogen/EWIEGA46WW/qrc_notepad.cpp: ../images/info.png
notepad_autogen/EWIEGA46WW/qrc_notepad.cpp: ../images/copy.png
notepad_autogen/EWIEGA46WW/qrc_notepad.cpp: ../images/bold.png
notepad_autogen/EWIEGA46WW/qrc_notepad.cpp: ../images/print.png
notepad_autogen/EWIEGA46WW/qrc_notepad.cpp: ../images/exit.png
notepad_autogen/EWIEGA46WW/qrc_notepad.cpp: ../images/edit_redo.png
notepad_autogen/EWIEGA46WW/qrc_notepad.cpp: ../images/pencil.png
notepad_autogen/EWIEGA46WW/qrc_notepad.cpp: ../images/create.png
notepad_autogen/EWIEGA46WW/qrc_notepad.cpp: ../images/save.png
notepad_autogen/EWIEGA46WW/qrc_notepad.cpp: ../images/edit_undo.png
notepad_autogen/EWIEGA46WW/qrc_notepad.cpp: ../images/cut.png
notepad_autogen/EWIEGA46WW/qrc_notepad.cpp: ../images/save_as.png
notepad_autogen/EWIEGA46WW/qrc_notepad.cpp: ../images/new.png
notepad_autogen/EWIEGA46WW/qrc_notepad.cpp: ../images/italic.png
notepad_autogen/EWIEGA46WW/qrc_notepad.cpp: C:/Qt/5.13.0/mingw73_64/bin/rcc.exe
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=D:\Raffaele\Documenti\GitHub\PDS_Project\Notepad_Project\CLion\notepad\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Automatic RCC for notepad.qrc"
	"D:\Programmi\JetBrains\CLion 2019.1.3\bin\cmake\win\bin\cmake.exe" -E cmake_autorcc D:/Raffaele/Documenti/GitHub/PDS_Project/Notepad_Project/CLion/notepad/cmake-build-debug/CMakeFiles/notepad_autogen.dir/RCCnotepadInfo.cmake Debug

CMakeFiles/notepad.dir/notepad_autogen/mocs_compilation.cpp.obj: CMakeFiles/notepad.dir/flags.make
CMakeFiles/notepad.dir/notepad_autogen/mocs_compilation.cpp.obj: CMakeFiles/notepad.dir/includes_CXX.rsp
CMakeFiles/notepad.dir/notepad_autogen/mocs_compilation.cpp.obj: notepad_autogen/mocs_compilation.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=D:\Raffaele\Documenti\GitHub\PDS_Project\Notepad_Project\CLion\notepad\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/notepad.dir/notepad_autogen/mocs_compilation.cpp.obj"
	C:\msys64\mingw64\bin\g++.exe  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\notepad.dir\notepad_autogen\mocs_compilation.cpp.obj -c D:\Raffaele\Documenti\GitHub\PDS_Project\Notepad_Project\CLion\notepad\cmake-build-debug\notepad_autogen\mocs_compilation.cpp

CMakeFiles/notepad.dir/notepad_autogen/mocs_compilation.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/notepad.dir/notepad_autogen/mocs_compilation.cpp.i"
	C:\msys64\mingw64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E D:\Raffaele\Documenti\GitHub\PDS_Project\Notepad_Project\CLion\notepad\cmake-build-debug\notepad_autogen\mocs_compilation.cpp > CMakeFiles\notepad.dir\notepad_autogen\mocs_compilation.cpp.i

CMakeFiles/notepad.dir/notepad_autogen/mocs_compilation.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/notepad.dir/notepad_autogen/mocs_compilation.cpp.s"
	C:\msys64\mingw64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S D:\Raffaele\Documenti\GitHub\PDS_Project\Notepad_Project\CLion\notepad\cmake-build-debug\notepad_autogen\mocs_compilation.cpp -o CMakeFiles\notepad.dir\notepad_autogen\mocs_compilation.cpp.s

CMakeFiles/notepad.dir/main.cpp.obj: CMakeFiles/notepad.dir/flags.make
CMakeFiles/notepad.dir/main.cpp.obj: CMakeFiles/notepad.dir/includes_CXX.rsp
CMakeFiles/notepad.dir/main.cpp.obj: ../main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=D:\Raffaele\Documenti\GitHub\PDS_Project\Notepad_Project\CLion\notepad\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/notepad.dir/main.cpp.obj"
	C:\msys64\mingw64\bin\g++.exe  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\notepad.dir\main.cpp.obj -c D:\Raffaele\Documenti\GitHub\PDS_Project\Notepad_Project\CLion\notepad\main.cpp

CMakeFiles/notepad.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/notepad.dir/main.cpp.i"
	C:\msys64\mingw64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E D:\Raffaele\Documenti\GitHub\PDS_Project\Notepad_Project\CLion\notepad\main.cpp > CMakeFiles\notepad.dir\main.cpp.i

CMakeFiles/notepad.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/notepad.dir/main.cpp.s"
	C:\msys64\mingw64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S D:\Raffaele\Documenti\GitHub\PDS_Project\Notepad_Project\CLion\notepad\main.cpp -o CMakeFiles\notepad.dir\main.cpp.s

CMakeFiles/notepad.dir/notepad.cpp.obj: CMakeFiles/notepad.dir/flags.make
CMakeFiles/notepad.dir/notepad.cpp.obj: CMakeFiles/notepad.dir/includes_CXX.rsp
CMakeFiles/notepad.dir/notepad.cpp.obj: ../notepad.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=D:\Raffaele\Documenti\GitHub\PDS_Project\Notepad_Project\CLion\notepad\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/notepad.dir/notepad.cpp.obj"
	C:\msys64\mingw64\bin\g++.exe  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\notepad.dir\notepad.cpp.obj -c D:\Raffaele\Documenti\GitHub\PDS_Project\Notepad_Project\CLion\notepad\notepad.cpp

CMakeFiles/notepad.dir/notepad.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/notepad.dir/notepad.cpp.i"
	C:\msys64\mingw64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E D:\Raffaele\Documenti\GitHub\PDS_Project\Notepad_Project\CLion\notepad\notepad.cpp > CMakeFiles\notepad.dir\notepad.cpp.i

CMakeFiles/notepad.dir/notepad.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/notepad.dir/notepad.cpp.s"
	C:\msys64\mingw64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S D:\Raffaele\Documenti\GitHub\PDS_Project\Notepad_Project\CLion\notepad\notepad.cpp -o CMakeFiles\notepad.dir\notepad.cpp.s

CMakeFiles/notepad.dir/notepad_autogen/EWIEGA46WW/qrc_notepad.cpp.obj: CMakeFiles/notepad.dir/flags.make
CMakeFiles/notepad.dir/notepad_autogen/EWIEGA46WW/qrc_notepad.cpp.obj: CMakeFiles/notepad.dir/includes_CXX.rsp
CMakeFiles/notepad.dir/notepad_autogen/EWIEGA46WW/qrc_notepad.cpp.obj: notepad_autogen/EWIEGA46WW/qrc_notepad.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=D:\Raffaele\Documenti\GitHub\PDS_Project\Notepad_Project\CLion\notepad\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/notepad.dir/notepad_autogen/EWIEGA46WW/qrc_notepad.cpp.obj"
	C:\msys64\mingw64\bin\g++.exe  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\notepad.dir\notepad_autogen\EWIEGA46WW\qrc_notepad.cpp.obj -c D:\Raffaele\Documenti\GitHub\PDS_Project\Notepad_Project\CLion\notepad\cmake-build-debug\notepad_autogen\EWIEGA46WW\qrc_notepad.cpp

CMakeFiles/notepad.dir/notepad_autogen/EWIEGA46WW/qrc_notepad.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/notepad.dir/notepad_autogen/EWIEGA46WW/qrc_notepad.cpp.i"
	C:\msys64\mingw64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E D:\Raffaele\Documenti\GitHub\PDS_Project\Notepad_Project\CLion\notepad\cmake-build-debug\notepad_autogen\EWIEGA46WW\qrc_notepad.cpp > CMakeFiles\notepad.dir\notepad_autogen\EWIEGA46WW\qrc_notepad.cpp.i

CMakeFiles/notepad.dir/notepad_autogen/EWIEGA46WW/qrc_notepad.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/notepad.dir/notepad_autogen/EWIEGA46WW/qrc_notepad.cpp.s"
	C:\msys64\mingw64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S D:\Raffaele\Documenti\GitHub\PDS_Project\Notepad_Project\CLion\notepad\cmake-build-debug\notepad_autogen\EWIEGA46WW\qrc_notepad.cpp -o CMakeFiles\notepad.dir\notepad_autogen\EWIEGA46WW\qrc_notepad.cpp.s

# Object files for target notepad
notepad_OBJECTS = \
"CMakeFiles/notepad.dir/notepad_autogen/mocs_compilation.cpp.obj" \
"CMakeFiles/notepad.dir/main.cpp.obj" \
"CMakeFiles/notepad.dir/notepad.cpp.obj" \
"CMakeFiles/notepad.dir/notepad_autogen/EWIEGA46WW/qrc_notepad.cpp.obj"

# External object files for target notepad
notepad_EXTERNAL_OBJECTS =

notepad.exe: CMakeFiles/notepad.dir/notepad_autogen/mocs_compilation.cpp.obj
notepad.exe: CMakeFiles/notepad.dir/main.cpp.obj
notepad.exe: CMakeFiles/notepad.dir/notepad.cpp.obj
notepad.exe: CMakeFiles/notepad.dir/notepad_autogen/EWIEGA46WW/qrc_notepad.cpp.obj
notepad.exe: CMakeFiles/notepad.dir/build.make
notepad.exe: C:/Qt/5.13.0/mingw73_64/lib/libQt5Networkd.a
notepad.exe: C:/Qt/5.13.0/mingw73_64/lib/libQt5PrintSupportd.a
notepad.exe: C:/Qt/5.13.0/mingw73_64/lib/libQt5Widgetsd.a
notepad.exe: C:/Qt/5.13.0/mingw73_64/lib/libQt5Guid.a
notepad.exe: C:/Qt/5.13.0/mingw73_64/lib/libQt5Cored.a
notepad.exe: CMakeFiles/notepad.dir/linklibs.rsp
notepad.exe: CMakeFiles/notepad.dir/objects1.rsp
notepad.exe: CMakeFiles/notepad.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=D:\Raffaele\Documenti\GitHub\PDS_Project\Notepad_Project\CLion\notepad\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Linking CXX executable notepad.exe"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\notepad.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/notepad.dir/build: notepad.exe

.PHONY : CMakeFiles/notepad.dir/build

CMakeFiles/notepad.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles\notepad.dir\cmake_clean.cmake
.PHONY : CMakeFiles/notepad.dir/clean

CMakeFiles/notepad.dir/depend: notepad_autogen/EWIEGA46WW/qrc_notepad.cpp
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" D:\Raffaele\Documenti\GitHub\PDS_Project\Notepad_Project\CLion\notepad D:\Raffaele\Documenti\GitHub\PDS_Project\Notepad_Project\CLion\notepad D:\Raffaele\Documenti\GitHub\PDS_Project\Notepad_Project\CLion\notepad\cmake-build-debug D:\Raffaele\Documenti\GitHub\PDS_Project\Notepad_Project\CLion\notepad\cmake-build-debug D:\Raffaele\Documenti\GitHub\PDS_Project\Notepad_Project\CLion\notepad\cmake-build-debug\CMakeFiles\notepad.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/notepad.dir/depend

