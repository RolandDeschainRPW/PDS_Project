cmake_minimum_required(VERSION 3.1.0)
project(notepad)

if (CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

include_directories(${PROJECT_SOURCE_DIR}/cmake-build-debug/notepad_autogen/include)
include_directories(${PROJECT_SOURCE_DIR}/include)

find_package(Qt5Core REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Network REQUIRED)
find_package(Qt5PrintSupport REQUIRED)

list(APPEND LIBRARIES
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets
        Qt5::Network
        Qt5::PrintSupport)
list(REMOVE_DUPLICATES LIBRARIES)

set(SOURCE_FILES
        src/main.cpp
        src/Notepad.cpp
        src/Symbol.cpp
        src/Message.cpp)

set(INCLUDE_FILES
        include/Notepad.h
        include/Symbol.h
        include/Message.h)

set(RESOURCE_FILES
        notepad.qrc)

set(UI_FILES
        src/notepad.ui)

add_executable(notepad
        ${UI_FILES}
        ${INCLUDE_FILES}
        ${SOURCE_FILES}
        ${RESOURCE_FILES})

target_link_libraries(notepad ${LIBRARIES})